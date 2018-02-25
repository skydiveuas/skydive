#include "CommDispatcher.hpp"

#include "CalibrationSettings.hpp"
#include "ControlSettings.hpp"
#include "RouteContainer.hpp"
#include "WifiConfiguration.hpp"

#include <string.h>

CommDispatcher::CommDispatcher(void)
{
    reset();
}

CommDispatcher::~CommDispatcher(void)
{
    cleanSignalDataBuffer();
}

void CommDispatcher::reset(void)
{
    preambleBufferCounter = 0;
    dataBufferCounter = 0;

    isPreambleActive = false;
    activePreambleType = IMessage::EMPTY;

    targetDataBufferCounter = 0;

    failedReceptionCounter = 0;
    sucessfullReceptionCounter = 0;

    receivingSignalData = false;
    receivedSignalData = SignalData::DUMMY;
    signalDataBuffer = NULL;
    signalDataPacketsToReceive = 0;
    signalDataPacketsReceived = 0;
}

const IMessage::PreambleType& CommDispatcher::getPreambleType(void) const
{
    return activePreambleType;
}

SignalData::Command CommDispatcher::getCommand(void) const
{
    return SignalData::parseCommand(dataBuffer);
}

SignalData::Parameter CommDispatcher::getParameter(void) const
{
    return SignalData::parseParameter(dataBuffer + IMessage::SIGNAL_COMMAND_SIZE);
}

SignalData CommDispatcher::getSignalData(void) const
{
    return SignalData(dataBuffer);
}

DebugData CommDispatcher::getDebugData(void) const
{
    return DebugData(dataBuffer);
}

ControlData CommDispatcher::getControlData(void) const
{
    return ControlData(dataBuffer);
}

SensorsData CommDispatcher::getSensorsData(void) const
{
    return SensorsData(dataBuffer);
}

AutopilotData CommDispatcher::getAutopilotData(void) const
{
    return AutopilotData(dataBuffer);
}

void CommDispatcher::getSignalDataObject(ISignalPayloadMessage& data)
{
    switch (data.getSignalDataCommand())
    {
    case SignalData::CALIBRATION_SETTINGS:
        reinterpret_cast<CalibrationSettings&>(data) = CalibrationSettings(signalDataBuffer);
        break;
    case SignalData::CONTROL_SETTINGS:
        reinterpret_cast<ControlSettings&>(data) = ControlSettings(signalDataBuffer);
        break;
    case SignalData::ROUTE_CONTAINER:
        reinterpret_cast<RouteContainer&>(data) = RouteContainer(signalDataBuffer);
        break;
    case SignalData::WIFI_CONFIGURATION:
        reinterpret_cast<WifiConfiguration&>(data) = WifiConfiguration(signalDataBuffer);
        break;
    default:
        // error should never reach this point
        break;
    }
    cleanSignalDataBuffer();
}

IMessage* CommDispatcher::retriveMessage(const IMessage::PreambleType preamble,
                                         const IMessage::MessageType expectedControlMessageType)
{
    switch (preamble)
    {
    case IMessage::CONTROL:
        switch (expectedControlMessageType)
        {
        case IMessage::DEBUG_DATA:
            return new DebugData(dataBuffer);

        case IMessage::CONTROL_DATA:
            return new ControlData(dataBuffer);

        case IMessage::SENSORS_DATA:
            return new SensorsData(dataBuffer);

        default: // error
            return NULL;
        }
        break;

    case IMessage::SIGNAL:
        return retriveSignalMessage();

    case IMessage::AUTOPILOT:
        return new AutopilotData(dataBuffer);

    default: // error
        return NULL;
    }
}

IMessage* CommDispatcher::retriveSignalMessage(void)
{
    const SignalData::Command command = getCommand();
    if (SignalData::hasPayload(command))
    {
        IMessage* result;
        switch (command)
        {
        case SignalData::CALIBRATION_SETTINGS_DATA:
            result = new CalibrationSettings(signalDataBuffer);
            break;

        case SignalData::CONTROL_SETTINGS_DATA:
            result = new ControlSettings(signalDataBuffer);
            break;

        case SignalData::ROUTE_CONTAINER_DATA:
            result = new RouteContainer(signalDataBuffer);
            break;

        case SignalData::WIFI_CONFIGURATION_DATA:
            result = new WifiConfiguration(signalDataBuffer);
            break;

        default: // error
            result = NULL;
        }
        cleanSignalDataBuffer();
        return result;
    }
    else
    {
        return new SignalData(dataBuffer);
    }
}

unsigned CommDispatcher::getSucessfullReceptions(void) const
{
    return sucessfullReceptionCounter;
}

unsigned CommDispatcher::getFailedReceptions(void) const
{
    return failedReceptionCounter;
}

void CommDispatcher::clearCounters(void)
{
    sucessfullReceptionCounter = 0;
    failedReceptionCounter = 0;
}

IMessage::PreambleType CommDispatcher::putChar(unsigned char data)
{
    // check for new preamble
    const IMessage::PreambleType newPreamble = updatePreamble(data);
    if (newPreamble != IMessage::EMPTY)
    {
        if (isPreambleActive)
        {
            // new preamble received when previous reception not ready, some fail
            //std::cout << "\nFAIL:  new preamble received when previous reception not ready\n\n";
#ifdef TRACER_H_
            Tracer::Trace("New preamble received when previous reception not ready");
#endif // TRACER_H_
            failedReceptionCounter++;
        }
        activatePreamble(newPreamble);
        return IMessage::EMPTY;
    }

    if (isPreambleActive)
    {
        // proceed processing data
        dataBuffer[dataBufferCounter] = data;
        dataBufferCounter++;

        if (dataBufferCounter >= targetDataBufferCounter)
        {
            // enough data in data buffer

            // check signal message condition
            if (activePreambleType == IMessage::SIGNAL
                    && dataBufferCounter == IMessage::SIGNAL_CONSTRAINT_SIZE)
            {
                // command from signal message just received, update target
                updataTargetDataSizeWithCommand();
                return IMessage::EMPTY;
            }

            // check CRC condition
            if (isValidMessageCrc())
            {
                // data received succesfully!
                sucessfullReceptionCounter++;
                IMessage::PreambleType result = activePreambleType;
                if (activePreambleType == IMessage::SIGNAL &&
                        SignalData::hasPayload(SignalData::parseCommand(dataBuffer)))
                {
                    handleSignalDataPayloadReception();
                    if (!isSignalDataComplete())
                    {
                        result = IMessage::EMPTY;
                    }
                    else
                    {
                        receivingSignalData = false;
                    }
                }
                else
                {
                    if (receivingSignalData)
                    {
                        //std::cout << "\nFAIL: receiving SignalData not ready\n\n";
                        failedReceptionCounter++;
#ifdef TRACER_H_
                        Tracer::Trace("Receiving SignalData not ready");
#endif // TRACER_H_
                    }
                    receivingSignalData = false;
                }
                deactivatePreamble();
                return result;
            }
            else
            {
                // something gone wrong, reset processor
                //std::cout << "\nFAIL: wrong CRC\n\n";
                failedReceptionCounter++;
#ifdef TRACER_H_
                Tracer::Trace("Wrong CRC");
#endif // TRACER_H_
                deactivatePreamble();
                return IMessage::EMPTY;
            }
        }
    }
    return IMessage::EMPTY;
}

IMessage::PreambleType CommDispatcher::updatePreamble(unsigned char data)
{
    // new preamble is arrived when all preamble bytes are the same
    // and equals to known preamlbe type
    // and the last received byte is equal to 0
    IMessage::PreambleType result = IMessage::EMPTY;
    if (data == 0)
    {
        bool allEquals = true;
        for (unsigned i = 0; i < IMessage::PREAMBLE_SIZE - 1; i++)
        {
            if (preambleBuffer[0] != preambleBuffer[i])
            {
                allEquals = false;
                break;
            }
        }
        if (allEquals)
        {
            result = IMessage::getPreabmleTypeByChar(preambleBuffer[0]);
        }
    }

    preambleBuffer[preambleBufferCounter] = data;
    preambleBufferCounter++;
    if (preambleBufferCounter >= IMessage::PREAMBLE_SIZE - 1)
    {
        preambleBufferCounter = 0;
    }

    return result;
}

void CommDispatcher::activatePreamble(IMessage::PreambleType preambleType)
{
    for (unsigned i = 0; i < IMessage::PREAMBLE_SIZE - 1; i++)
    {
        preambleBuffer[0] = 0;
    }

    isPreambleActive = true;
    activePreambleType = preambleType;

    dataBufferCounter = 0;
    targetDataBufferCounter = IMessage::getPayloadSizeByType(preambleType);
    if (activePreambleType != IMessage::SIGNAL)
    {
        targetDataBufferCounter += IMessage::CRC_SIZE;
    }
}

void CommDispatcher::updataTargetDataSizeWithCommand(void)
{
    const SignalData::Command command = SignalData::parseCommand(dataBuffer);
    if (SignalData::hasPayload(command))
    {
        targetDataBufferCounter += IMessage::SIGNAL_DATA_PAYLOAD_SIZE + IMessage::CRC_SIZE;
    }
    else
    {
        targetDataBufferCounter += IMessage::CRC_SIZE;
    }
}

void CommDispatcher::initSignalDataPayloadReception(const SignalData::Command& command, const unsigned short allPackets)
{
    cleanSignalDataBuffer();
    receivedSignalData = command;
    signalDataBuffer = new unsigned char[allPackets * IMessage::SIGNAL_DATA_PAYLOAD_SIZE];
    signalDataPacketsToReceive = allPackets;
    receivingSignalData = true;
    signalDataPacketsReceived = 0;
}

void CommDispatcher::handleSignalDataPayloadReception(void)
{
    const SignalData::Command command = SignalData::parseCommand(dataBuffer);
    const unsigned short allPackets = SignalData::parseAllPacketsNumber(dataBuffer);
    const unsigned short packetNumber = SignalData::parseActualPacketNumber(dataBuffer);

    if (!receivingSignalData || receivedSignalData != command)
    {
        // new data or another data was being received
        initSignalDataPayloadReception(command, allPackets);
    }
    // put data to bufer in to reported position
    memcpy(signalDataBuffer + packetNumber * IMessage::SIGNAL_DATA_PAYLOAD_SIZE,
           dataBuffer + IMessage::SIGNAL_CONSTRAINT_SIZE,
           IMessage::SIGNAL_DATA_PAYLOAD_SIZE);
    signalDataPacketsReceived++;
}

bool CommDispatcher::isSignalDataComplete(void)
{
    return signalDataPacketsToReceive == signalDataPacketsReceived;
}

void CommDispatcher::cleanSignalDataBuffer(void)
{
    if (signalDataBuffer != NULL)
    {
        delete[] signalDataBuffer;
        signalDataBuffer = NULL;
    }
}

bool CommDispatcher::isValidMessageCrc(void) const
{
    const unsigned short crc = IMessage::computeCrc16(dataBuffer, dataBufferCounter - 2);
    unsigned char crc1 = (unsigned char)(crc & 0xff);
    unsigned char crc2 = (unsigned char)((crc >> 8) & 0xff);
    return	dataBuffer[targetDataBufferCounter - 2] == crc1
            && dataBuffer[targetDataBufferCounter - 1] == crc2;
}

void CommDispatcher::deactivatePreamble(void)
{
    isPreambleActive = false;
    activePreambleType = IMessage::EMPTY;
    dataBufferCounter = 0;
    targetDataBufferCounter = 0;
}
