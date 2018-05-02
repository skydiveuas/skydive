#include "SkyDevice.hpp"

#include "SkyException.hpp"

#include "PilotEvent.hpp"

#include "IdleAction.hpp"

#include <stdlib.h>
#include <functional>

SkyDevice::SkyDevice(ISkyDeviceMonitor* const _monitor,
                     const double _pingFreq,
                     const double _controlFreq,
                     const double _connectionTimeout) :
    monitor(_monitor),
    pingFreq(_pingFreq),
    controlFreq(_controlFreq),
    connectionTimeoutFreq(1 / _connectionTimeout),
    pingTimer(monitor->createTimer(std::bind(&SkyDevice::pingTimerHandler, this))),
    connetionTimer(monitor->createTimer(std::bind(&SkyDevice::connectionTimerHandler, this)))
{
    action = std::make_shared<IdleAction>(this);

    std::srand(std::time(0));
}

void SkyDevice::pushPilotEvent(const PilotEvent* const pilotEvent)
{
    pushPilotEvent(std::unique_ptr<const PilotEvent>(pilotEvent));
}

void SkyDevice::pushPilotEvent(std::unique_ptr<const PilotEvent> pilotEvent)
{
    notifyPilotEvent(action, pilotEvent.get());
}

ISkyDeviceAction::Type SkyDevice::getState(void) const
{
    return action->getType();
}

void SkyDevice::notifyPilotEvent(std::shared_ptr<ISkyDeviceAction> guard,
                                    const PilotEvent* const operatorEvent)
{
    monitor->trace("Handling pilot event: " + operatorEvent->toString() + " at: " + guard->getName());
    std::unique_lock<std::mutex>(actionLock);
    guard->handleUserEvent(*operatorEvent);
}

void SkyDevice::notifyReception(std::shared_ptr<ISkyDeviceAction> guard,
                                const IMessage* const message)
{
    //monitor->trace("HandleReception reception: " + message->getMessageName() + " at: " + guard->getName());

    receptionFeed = true;
    if (connectionLost)
    {
        connectionLost = false;
        monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CONNECTION_RECOVERED));
    }

    try
    {
        std::unique_lock<std::mutex>(actionLock);
        guard->baseHandleReception(std::unique_ptr<const IMessage>(message));
    }
    catch (SkyException e)
    {
        handleError(e.message());
    }
}

void SkyDevice::handleError(const std::string& message)
{
    monitor->trace("SkyDevice::handleError: " + message);
    enablePingTask(false);
    enableConnectionTimeoutTask(false);
    std::shared_ptr<ISkyDeviceAction> newAction = std::make_shared<IdleAction>(this);
    std::unique_lock<std::mutex>(actionLock);
    action.swap(newAction);
    monitor->notifyDeviceEvent(new DeviceEventMessage(DeviceEventMessage::ERROR, message));
    interface->disconnect();
}

void SkyDevice::pingTimerHandler(void)
{
    sentPingValue = std::rand() * 163;
    sentPingTime = clock();
    send(SignalData(SignalData::PING_VALUE, sentPingValue));
}

void SkyDevice::handlePong(const SignalData& signalData) const
{
    if (signalData.getParameterValue() == sentPingValue)
    {
        unsigned ping = (unsigned)(((clock() - sentPingTime) / 2.0f) + 0.5f);
        monitor->notifyDeviceEvent(new DeviceEventConnectionStatus(
                                       ping,
                                       dispatcher.getSucessfullReceptions(),
                                       dispatcher.getFailedReceptions()));
    }
}

void SkyDevice::connectionTimerHandler(void)
{
    if (false == receptionFeed)
    {
        // connection timeout occured
        if (false == connectionLost)
        {
            // first time
            connectionLost = true;
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CONNECTION_LOST));
        }
    }
    receptionFeed = false;
}

void SkyDevice::onConnected()
{
    monitor->trace("SkyDevice::onConnected");
    std::unique_lock<std::mutex>(actionLock);
    action->start();
}

void SkyDevice::onDisconnected()
{
    monitor->trace("SkyDevice::onDisconnected");
}

void SkyDevice::onError(const std::string& message)
{
    handleError(message);
}

void SkyDevice::onReceived(const unsigned char* data, const size_t length)
{
    IMessage::PreambleType receivedPreamble;
    for (unsigned i = 0; i < length; i++)
    {
        receivedPreamble = dispatcher.putChar(data[i]);
        if (IMessage::EMPTY != receivedPreamble)
        {
            notifyReception(action, dispatcher.retriveMessage(
                                receivedPreamble, action->getExpectedControlMessageType()));
        }
    }
}

ISkyDeviceMonitor* SkyDevice::getMonitor(void)
{
    return monitor;
}

bool SkyDevice::setupProtocolVersion(const unsigned version)
{
    monitor->trace("Setting up protocol version: " + std::to_string(version) +
                   ", compiled version: " + std::to_string(IMessage::PROTOCOL_VERSION));
    // TODO currenty protocol version handling is not supported
    return IMessage::PROTOCOL_VERSION == version;
}

void SkyDevice::startAction(ISkyDeviceAction* newAction, bool immediateStart)
{
    monitor->trace("Starting new action: " + newAction->getName());

    if (false == action->isActionDone())
    {
        std::string message = "Previous action (" + action->getName() +
                ") not done, when performing: " + newAction->getName() + ".";
        __SKY_EXCEPTION__(message.c_str());
    }

    std::shared_ptr<ISkyDeviceAction> newActionGuard(newAction);

    std::unique_lock<std::mutex>(actionLock);
    action->end();

    action.swap(newActionGuard);

    if (immediateStart)
    {
        action->start();
    }
}

void SkyDevice::onPongReception(const SignalData& pong)
{
    handlePong(pong);
}

void SkyDevice::send(const IMessage& message)
{
    message.serializeMessage(messageBuildingBuffer);
    interface->send(messageBuildingBuffer, message.getMessageSize());
}

void SkyDevice::send(const ISignalPayloadMessage& message)
{
    ISignalPayloadMessage::MessagesBuilder builder(&message);
    while (builder.hasNext())
    {
        builder.getNext(messageBuildingBuffer);
        interface->send(messageBuildingBuffer, IMessage::SIGNAL_DATA_MESSAGE_SIZE);
    }
}

void SkyDevice::enablePingTask(bool enable)
{
    if (enable)
    {
        monitor->trace("Starting ping task");
        pingTimer->start(pingFreq);
    }
    else
    {
        monitor->trace("Ending ping task");
        pingTimer->stop();
    }
}

void SkyDevice::enableConnectionTimeoutTask(bool enable)
{
    if (enable)
    {
        monitor->trace("Starting connection timeout task");
        connetionTimer->start(connectionTimeoutFreq);
    }
    else
    {
        monitor->trace("Ending connection timeout task");
        connetionTimer->stop();
    }
}

void SkyDevice::connectInterface(ISkyCommInterface* _interface)
{
    interface = _interface;
    interface->setListener(this);
    interface->connect();
}

void SkyDevice::disconnectInterface(void)
{
    enablePingTask(false);
    enableConnectionTimeoutTask(false);
    interface->disconnect();
}
