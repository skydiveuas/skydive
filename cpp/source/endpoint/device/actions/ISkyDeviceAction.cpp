#include "ISkyDeviceAction.hpp"

#include "OperatorEvent.hpp"
#include "SKyException.hpp"

#include <memory>

ISkyDeviceAction::Listener::~Listener(void)
{
}

ISkyDeviceAction::ISkyDeviceAction(Listener* const _listener):
    listener(_listener),
    monitor(listener->getMonitor())
{
    wasSignalReceptionProcedure = false;
    signalTimer = monitor->createTimer(std::bind(&ISkyDeviceAction::baseHandleTimeout, this));
}

ISkyDeviceAction::~ISkyDeviceAction()
{
    delete signalTimer;
}

void ISkyDeviceAction::end(void)
{
    // nothing to do in generic action end
}

void ISkyDeviceAction::baseHandleReception(std::unique_ptr<const IMessage> message)
{
    // filter any SignalData to propper reception handler
    if (IMessage::SIGNAL_DATA == message->getMessageType())
    {
        handleReception(reinterpret_cast<const SignalData&>(*message));
    }
    else
    {
        handleReception(*message);
        // after successfull reception handling release message from RAII
        // it will be passed forward as UavEvent parameter
        message.release();
    }
}

void ISkyDeviceAction::handleUserEvent(const OperatorEvent& event)
{
    except("Unexpected user uav event received", event);
}

IMessage::MessageType ISkyDeviceAction::getExpectedControlMessageType(void) const
{
    return IMessage::DEBUG_DATA;
}

std::string ISkyDeviceAction::getName(void) const
{
    return toString(getType()) + " @ " + getStateName();
}

std::string ISkyDeviceAction::toString(const Type type)
{
    switch (type)
    {
    case IDLE_ACTION: return "IDLE_ACTION";
    case CONNECT: return "CONNECT";
    case UPGRADE: return "UPGRADE";
    case WHO_AM_I: return "WHO_AM_I";
    case DISCONNECT: return "DISCONNECT";
    case APP: return "APP";
    case FLIGHT_INITIALIZATION: return "FLIGHT_INITIALIZATION";
    case FLIGHT: return "FLIGHT";
    case ACCEL_CALIB: return "ACCEL_CALIB";
    case MAGNET_CALIB: return "MAGNET_CALIB";
    case UPLOAD_SIGNAL_PAYLOAD: return "UPLOAD_SIGNAL_PAYLOAD";
    case DOWNLOAD_SIGNAL_PAYLOAD: return "DOWNLOAD_SIGNAL_PAYLOAD";
    case SENSORS_LOGGER: return "SENSORS_LOGGER";
    case RADIO_CHECK: return "RADIO_CHECK";
    case RADIO_CALIB: return "RADIO_CALIB";
    case ESC_CALIB: return "ESC_CALIB";
    case RESET: return "RESET";
    default: __SKY_EXCEPTION__("ICommAction::toString::Unexpected action type");
    }
}

void ISkyDeviceAction::handleReception(const SignalData& message)
{
    if (wasSignalReceptionProcedure)
    {
        if (message.getCommand() == expectedSignalCommand)
        {
            endSignalTimeout();
            monitor->trace("HandleSignalReception with " + getName() +
                           " param: " + SignalData::toString(message.getParameter()));
            handleSignalReception(message.getParameter());
        }
    }
    else
    {
        // push message back to generic hanlder
        handleReception(reinterpret_cast<const IMessage&>(message));
    }
}

void ISkyDeviceAction::handleSignalReception(const Parameter parameter)
{
    except("Unexpected handleSignalReception event ocured", parameter);
}

void ISkyDeviceAction::baseHandleTimeout(void)
{
    signalTimer->stop();
    if (wasSignalReceptionProcedure)
    {
        listener->onError("Timeout waiting for " + SignalData::toString(expectedSignalCommand) +
                          (expectedSignalCommand == sentSignal.getCommand() ?
                               " after sending: (" + sentSignal.toString() + ")" : ""));
    }
    else if (wasSignalPayloadReceptionProcedure)
    {
        receptionErrors++;
        monitor->trace("Timeout when receiving over signal payload, errors: " +
                       std::to_string(receptionErrors) + " / " + std::to_string(MAX_SIGNAL_PAYLOAD_RECEPTION_ERRORS));
        if (receptionErrors < MAX_SIGNAL_PAYLOAD_RECEPTION_ERRORS)
        {
            listener->send(SignalData(receivedSignalPayload, SignalData::TIMEOUT));
            signalTimer->start(1000.0 / (1.5 * DEFAULT_SIGNAL_TIMEOUT));
        }
        else
        {
            listener->onError("Error while receiving signal payload: " + SignalData::toString(receivedSignalPayload));
        }
    }
    else
    {
        handleTimeout();
    }
}

void ISkyDeviceAction::handleTimeout(void)
{
    except("Unexpected timeout event occurred");
}

void ISkyDeviceAction::sendSignal(const Command command, const Parameter parameter, const unsigned timeout)
{
    sentSignal = SignalData(command, parameter);
    startSignalTimeout(command, timeout);
    listener->send(sentSignal);
}

void ISkyDeviceAction::startSignalTimeout(const Command expectedCommand, const unsigned timeout)
{
    monitor->trace("Starting signal reception " + SignalData::toString(expectedCommand) +
                   " with timeout: " + std::to_string(timeout) + " ms");
    expectedSignalCommand = expectedCommand;
    wasSignalReceptionProcedure = true;
    startSignalTimeoutTimer(timeout);
}

void ISkyDeviceAction::startSignalTimeoutTimer(const unsigned timeout)
{
    signalTimer->start(1000.0 / timeout);
}

void ISkyDeviceAction::endSignalTimeout(void)
{
    signalTimer->stop();
    wasSignalReceptionProcedure = false;
    wasSignalPayloadReceptionProcedure = false;
    expectedSignalCommand = SignalData::DUMMY;
    sentSignal = SignalData(SignalData::DUMMY, SignalData::DUMMY_PARAMETER);
}

void ISkyDeviceAction::initializeSignalPayloadReception(const SignalData::Command& command)
{
    monitor->trace("ICommAction::initializeSignalPayloadReception: " + SignalData::toString(command));
    receivedSignalPayload = command;
    wasSignalPayloadReceptionProcedure = true;
    receptionErrors = 0;
    signalTimer->start(1000.0 / (1.5 * DEFAULT_SIGNAL_TIMEOUT));
}

bool ISkyDeviceAction::handleSignalPayloadReception(const IMessage& message)
{
    if (message.isSignalPayloadMessage())
    {
        signalTimer->stop();
        const ISignalPayloadMessage& signalData = reinterpret_cast<const ISignalPayloadMessage&>(message);
        if (receivedSignalPayload == signalData.getSignalDataCommand())
        {
            if (signalData.isValid())
            {
                endSignalTimeout();
                listener->send(SignalData(receivedSignalPayload, SignalData::ACK));
                return true;
            }
            else
            {
                receptionErrors++;
                monitor->trace("Invalid data received over signal payload, errors: " +
                               std::to_string(receptionErrors) + " / " + std::to_string(MAX_SIGNAL_PAYLOAD_RECEPTION_ERRORS));
                if (receptionErrors < MAX_SIGNAL_PAYLOAD_RECEPTION_ERRORS)
                {
                    listener->send(SignalData(receivedSignalPayload, SignalData::DATA_INVALID));
                    signalTimer->start(1000.0 / (1.5 * DEFAULT_SIGNAL_TIMEOUT));
                }
                else
                {
                    except("Error while receiving signal payloada: " + SignalData::toString(receivedSignalPayload));
                }
            }
        }
    }
    else
    {
        except("Unexpected message received when waiting for signal payload data");
    }
    return false;
}

void ISkyDeviceAction::handleIdleReception(const IMessage& message) const
{
    if (!(getExpectedControlMessageType() == message.getMessageType() || isPingMessage(message)))
    {
        except("Unexpected message in idle reception", message);
    }
}

bool ISkyDeviceAction::isPingMessage(const IMessage& received, int& value) const
{
    if (IMessage::SIGNAL_DATA == received.getMessageType()
            && SignalData::PING_VALUE == reinterpret_cast<const SignalData&>(received).getCommand())
    {
        value = reinterpret_cast<const SignalData&>(received).getParameter();
        return true;
    }
    else
    {
        return false;
    }
}

bool ISkyDeviceAction::isPingMessage(const IMessage& received) const
{
    int dummyValue;
    return isPingMessage(received, dummyValue);
}

bool ISkyDeviceAction::matchSignalData(const SignalData& expected, const IMessage& received) const
{
    return IMessage::SIGNAL_DATA == received.getMessageType()
            && expected == reinterpret_cast<const SignalData&>(received);
}

bool ISkyDeviceAction::matchSignalData(const SignalData::Command command,
                                  const SignalData::Parameter parameter,
                                  const IMessage& received) const
{
    return matchSignalData(SignalData(command, parameter), received);
}

void ISkyDeviceAction::except(const std::string& message) const
{
    std::string msg = message + " at " + getName();
    __SKY_EXCEPTION__(msg.c_str());
}

void ISkyDeviceAction::except(const std::string& message, const IMessage& received) const
{
    std::string msg = message + " at " + getName() + " message: " + received.toString();
    __SKY_EXCEPTION__(msg.c_str());
}

void ISkyDeviceAction::except(const std::string& message, const OperatorEvent& event) const
{
    std::string msg = message + " at " + getName() + " event: " + event.toString();
    __SKY_EXCEPTION__(msg.c_str());
}

void ISkyDeviceAction::except(const std::string& message, const SignalData::Parameter parameter) const
{
    std::string msg = message + " at " + getName() + " parameter: " + SignalData::toString(parameter);
    __SKY_EXCEPTION__(msg.c_str());
}
