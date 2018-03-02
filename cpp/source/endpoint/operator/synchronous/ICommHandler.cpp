#include "ICommHandler.hpp"

#include <string.h>

#ifdef __SKYDIVE_USE_STL__

#include "Exception.hpp"
#include "SignalData.hpp"

#include <string>

#endif //__SKYDIVE_USE_STL__

ICommHandler::ICommHandler(void) :
    buildingTempBuffer(new unsigned char[IMessage::MAX_DATA_SIZE])
{
}

ICommHandler::~ICommHandler(void)
{
    delete[] buildingTempBuffer;
}

void ICommHandler::initialize(ICommInterface* _commInterface)
{
    commInterface = _commInterface;
    commDispatcher.reset();
    clearCounters();
    sentMessages = 0;
}

void ICommHandler::reset(void)
{
    commInterface->flush();
    commDispatcher.reset();
    clearCounters();
    sentMessages = 0;
}

const ICommInterface* ICommHandler::getCommHandle(void) const
{
    return commInterface;
}

IMessage::PreambleType ICommHandler::proceedReceiving(void)
{
    unsigned char data;
    while (commInterface->getData(&data))
    {
        IMessage::PreambleType received = commDispatcher.putChar(data);
        if (received != IMessage::EMPTY)
        {
            // if stream processor reports data received return immediately
            return received;
        }
    }
    return IMessage::EMPTY;
}

bool ICommHandler::send(const unsigned char* data, const unsigned dataSize)
{
    sentMessages++;
    return commInterface->sendData(data, dataSize);
}

bool ICommHandler::send(const IMessage& message)
{
    message.serializeMessage(buildingTempBuffer);
    return send(buildingTempBuffer, message.getMessageSize());
}

bool ICommHandler::send(const ISignalPayloadMessage& message)
{
    return sendSignalData(&message);
}

SignalData ICommHandler::getSignalData(void) const
{
    return commDispatcher.getSignalData();
}

SignalData::Command ICommHandler::getCommand(void) const
{
    return commDispatcher.getCommand();
}

SignalData::Parameter ICommHandler::getParameter(void) const
{
    return commDispatcher.getParameter();
}

DebugData ICommHandler::getDebugData(void) const
{
    return commDispatcher.getDebugData();
}

ControlData ICommHandler::getControlData(void) const
{
    return commDispatcher.getControlData();
}

SensorsData ICommHandler::getSensorsData(void) const
{
    return commDispatcher.getSensorsData();
}

AutopilotData ICommHandler::getAutopilotData(void) const
{
    return commDispatcher.getAutopilotData();
}

void ICommHandler::getSignalDataObject(ISignalPayloadMessage& data)
{
    commDispatcher.getSignalDataObject(data);
}

unsigned ICommHandler::getSentMessages(void) const
{
    return sentMessages;
}

unsigned ICommHandler::getReceivedMessage(void) const
{
    return commDispatcher.getSucessfullReceptions();
}

unsigned ICommHandler::getReceptionFailes(void) const
{
    return commDispatcher.getFailedReceptions();
}

void ICommHandler::clearCounters(void)
{
    sentMessages = 0;
    commDispatcher.clearCounters();
}

bool ICommHandler::sendCommand(const SignalData& command)
{
    return send(command);
}

bool ICommHandler::waitForCommand(const SignalData& command, const unsigned timeout)
{
    resetTimer();
    while (true) // infinite loop
    {
        if (proceedReceiving() == IMessage::SIGNAL)
        {
            if (command == getSignalData())
            {
                return true;
            }
        }
        if (getTimerValue() > timeout)
        {
            return false;
        }
        holdThread(10);
    }
}

bool ICommHandler::waitForAnyCommand(SignalData& command, const unsigned timeout)
{
    resetTimer();
    while (true) // infinite loop
    {
        if (proceedReceiving() == IMessage::SIGNAL)
        {
            if (!SignalData::hasPayload(getCommand()))
            {
                command = getSignalData();
                return true;
            }
        }
        if (getTimerValue() > timeout)
        {
            return false;
        }
        holdThread(10);
    }
}

bool ICommHandler::waitForAnyParameter(const Command command, Parameter& parameter, const unsigned timeout)
{
    resetTimer();
    while (true) // infinite loop
    {
        if (proceedReceiving() == IMessage::SIGNAL)
        {
            if (command == getCommand())
            {
                parameter = getParameter();
                return true;
            }
        }
        if (getTimerValue() > timeout)
        {
            return false;
        }
        holdThread(10);
    }
}

bool ICommHandler::sendCommandGetAnyResponse(const SignalData& command, SignalData& response)
{
    if (!send(command))
    {
        return false;
    }
    return waitForAnyCommand(response);
}

bool ICommHandler::sendCommandGetAnyParameter(const SignalData& command, const Command responseCommand, Parameter& response)
{
    if (!send(command))
    {
        return false;
    }
    return waitForAnyParameter(responseCommand, response);
}

bool ICommHandler::sendCommandGetResponse(const SignalData& command, const SignalData& response)
{
    Parameter receivedParameter;
    return sendCommandGetAnyParameter(command, response.getCommand(), receivedParameter)
            && receivedParameter == response.getParameter();
}

bool ICommHandler::readCommandAndRespond(const SignalData& command, const SignalData& response)
{
    if (waitForCommand(command))
    {
        return sendCommand(response);
    }
    else
    {
        return false;
    }
}

bool ICommHandler::sendDataProcedure(const ISignalPayloadMessage& data)
{
    for (unsigned ret = 0; ret < MAX_RETRANSMISSION + 1; ret++)
    {
        if (!send(data))
        {
            // sending data failed
#ifdef TRACER_H_
            Tracer::Trace("sendDataProcedure::sending data failed", true);
#endif
            return false;
        }
        SignalData receivedCommand;
        if (waitForAnyCommand(receivedCommand, DEFAULT_TIMEOUT + 500))
        {
            if (receivedCommand.getCommand() == data.getSignalDataCommand())
            {
                switch (receivedCommand.getParameter())
                {
                case SignalData::ACK:
                    // sending data successful
                    return true;

                case SignalData::DATA_INVALID:
#ifdef TRACER_H_
                    Tracer::Trace("sendDataProcedure::DATA_INVALID, retransmitting", true);
#endif
                    // retransmit data
                    break;

                case SignalData::TIMEOUT:
#ifdef TRACER_H_
                    Tracer::Trace("sendDataProcedure::TIMEOUT, retransmitting", true);
#endif
                    // retransmit data
                    break;

                default:
                    // bad parameter received
#ifdef TRACER_H_
                    Tracer::Trace("sendDataProcedure::Bad parameter received!", true);
#endif
                    return false;
                }
            }
            else
            {
                // bad command received
#ifdef TRACER_H_
                Tracer::Trace("sendDataProcedure::Bad command received!", true);
#endif
                return false;
            }
        }
        else
        {
            // timeout waiting for data
#ifdef TRACER_H_
            Tracer::Trace("sendDataProcedure::timeout waiting for command, wait for retransmission", true);
#endif
            return false;
        }
    }
#ifdef TRACER_H_
    Tracer::Trace("sendDataProcedure::failed after max retransmissions", true);
#endif
    // failed after max retransmissions
    return false;
}

bool ICommHandler::receiveDataProcedure(ISignalPayloadMessage& data)
{
    resetTimer();
    unsigned retransmissionCounter = 0;
    while (true) // infinite loop
    {
        if (proceedReceiving() == IMessage::SIGNAL
                && getCommand() == data.getSignalDataType())
        {
            getSignalDataObject(data);
            if (data.isValid())
            {
                send(SignalData(data.getSignalDataCommand(), SignalData::ACK));
                return true;
            }
            else
            {
#ifdef TRACER_H_
                Tracer::Trace("receiveDataProcedure::DATA_INVALID!", true);
#endif
                retransmissionCounter++;
                if (retransmissionCounter >= MAX_RETRANSMISSION + 1)
                {
                    // failed after max retransmissions
                    break;
                }
                send(SignalData(data.getSignalDataCommand(), SignalData::DATA_INVALID));
                resetTimer();
            }
        }
        if (getTimerValue() > DEFAULT_TIMEOUT)
        {
#ifdef TRACER_H_
            Tracer::Trace("receiveDataProcedure::TIMEOUT!", true);
#endif
            retransmissionCounter++;
            if (retransmissionCounter >= MAX_RETRANSMISSION + 1)
            {
                // failed after max retransmissions
                break;
            }
            send(SignalData(data.getSignalDataCommand(), SignalData::TIMEOUT));
            resetTimer();
        }
        holdThread(10);
    }
#ifdef TRACER_H_
    Tracer::Trace("receiveDataProcedure::Max retransmissions reached!", true);
#endif
    return false;
}

bool ICommHandler::sendSignalData(const ISignalPayloadMessage* const data)
{
    ISignalPayloadMessage::MessagesBuilder builder(data);

    unsigned char message[IMessage::SIGNAL_DATA_MESSAGE_SIZE];

    while (builder.hasNext())
    {
        builder.getNext(message);

        // sending message
        resetTimer();
        while (!send(message, IMessage::SIGNAL_DATA_MESSAGE_SIZE))
        {
            // according to SkyDive Communication Protocol - max message size is 64 bytes
            // so timeout should be long enough to send this amount of data
            // assuming USART as worst case interface and baudrate as 115200
            // this is about 4ms, so timeout is set to 10 ms
            if (getTimerValue() >= 10)
            {
#ifdef TRACER_H_
                Tracer::Trace("Sending payload signal data failed after retries, ERROR!", true);
#endif
                return false;
            }
            holdThread(2);
        }
    }
    return true;
}

#ifdef __SKYDIVE_USE_STL__

void ICommHandler::sendCommandEx(const SignalData& command)
{
    if (!sendCommand(command))
    {
        std::string message = "Sending command: ("
                + command.toString()
                + ") failed!";
        __RL_EXCEPTION__(message.c_str());
    }
}

void ICommHandler::waitForCommandEx(const SignalData& command, const unsigned timeout)
{
    if (!waitForCommand(command, timeout))
    {
        std::string message = "Waiting for command: ("
                + command.toString()
                + ") timeout!";
        __RL_EXCEPTION__(message.c_str());
    }
}

SignalData ICommHandler::waitForAnyCommandEx(const unsigned timeout)
{
    SignalData result;
    if (!waitForAnyCommand(result, timeout))
    {
        __RL_EXCEPTION__("Waiting for any command timeout!");
    }
    return result;
}

ICommHandler::Parameter ICommHandler::waitForAnyParameterEx(const Command command, const unsigned timeout)
{
    Parameter result;
    if (!waitForAnyParameter(command, result, timeout))
    {
        std::string message = "Waiting for any parameter with command: ("
                + SignalData::toString(command)
                + ") timeout!";
        __RL_EXCEPTION__(message.c_str());
    }
    return result;
}

SignalData ICommHandler::sendCommandGetAnyResponseEx(const SignalData& command)
{
    SignalData result;
    if (!sendCommandGetAnyResponse(command, result))
    {
        std::string message = "Send command: ("
                + command.toString()
                + ") with any response timeout!";
        __RL_EXCEPTION__(message.c_str());
    }
    return result;
}

ICommHandler::Parameter ICommHandler::sendCommandGetAnyParameterEx(const SignalData& command, const Command responseCommand)
{
    Parameter result;
    if (!sendCommandGetAnyParameter(command, responseCommand, result))
    {
        std::string message = "Send command: ("
                + command.toString()
                + ") with any response with command: ("
                + SignalData::toString(responseCommand)
                + " timeout!";
        __RL_EXCEPTION__(message.c_str());
    }
    return result;
}

void ICommHandler::sendCommandGetResponseEx(const SignalData& command, const SignalData& response)
{
    if (!sendCommandGetResponse(command, response))
    {
        std::string message = "Send command: ("
                + command.toString()
                + ") with response: ("
                + response.toString()
                + ") failed!";
        __RL_EXCEPTION__(message.c_str());
    }
}

void ICommHandler::readCommandAndRespondEx(const SignalData& command, const SignalData& response)
{
    if (!sendCommandGetResponse(command, response))
    {
        std::string message = "Read command: ("
                + command.toString()
                + ") and respond: ("
                + response.toString()
                + ") failed!";
        __RL_EXCEPTION__(message.c_str());
    }
}

void ICommHandler::sendDataProcedureEx(const ISignalPayloadMessage& signalPayloadMessage)
{
    if (!sendDataProcedure(signalPayloadMessage))
    {
        const std::string message = "Sending "
                + SignalData::toString(signalPayloadMessage.getSignalDataCommand())
                + " failed!";
        __RL_EXCEPTION__(message.c_str());
    }
}

CalibrationSettings ICommHandler::receiveCalibarionSettingsEx()
{
    CalibrationSettings result;
    if (!receiveDataProcedure(result))
    {
        __RL_EXCEPTION__("Receiving calibration settings failed!");
    }
    return result;
}

ControlSettings ICommHandler::receiveControlSettingsEx()
{
    ControlSettings result;
    if (!receiveDataProcedure(result))
    {
        __RL_EXCEPTION__("Receiving control settings failed!");
    }
    return result;
}

RouteContainer ICommHandler::receiveRouteContainerEx()
{
    RouteContainer result;
    if (!receiveDataProcedure(result))
    {
        __RL_EXCEPTION__("Receiving route container failed!");
    }
    return result;
}

#endif //__SKYDIVE_USE_STL__
