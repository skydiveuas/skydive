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

void SkyDevice::pushOperatorEvent(const PilotEvent* const operatorEvent)
{
    pushOperatorEvent(std::unique_ptr<const PilotEvent>(operatorEvent));
}

void SkyDevice::pushOperatorEvent(std::unique_ptr<const PilotEvent> operatorEvent)
{
    notifyOperatorEvent(action, operatorEvent.get());
}

ISkyDeviceAction::Type SkyDevice::getState(void) const
{
    if (nullptr == action)
    {
        return ISkyDeviceAction::IDLE_ACTION;
    }
    else
    {
        return action->getType();
    }
}

void SkyDevice::notifyOperatorEvent(std::shared_ptr<ISkyDeviceAction> guard,
                                    const PilotEvent* const operatorEvent)
{
    monitor->trace("Handling user event: " + operatorEvent->toString() + " at: " + guard->getName());
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
    action.swap(newAction);
    monitor->notifyDeviceEvent(new DeviceEventMessage(DeviceEventMessage::ERROR, message));
    interface->disconnect();
}

void SkyDevice::pingTimerHandler(void)
{
    sentPingValue = std::rand();
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

void SkyDevice::onDataReceived(const unsigned char* data, const unsigned dataSize)
{
    IMessage::PreambleType receivedPreamble;
    for (unsigned i = 0; i < dataSize; i++)
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

void SkyDevice::startAction(ISkyDeviceAction* newAction, bool immediateStart)
{
    monitor->trace("Starting new action: " + newAction->getName());

    if (false == action->isActionDone())
    {
        std::string message = "Previous action (" + action->getName() +
                ") not done, when performing: " + newAction->getName() + ".";
        __SKY_EXCEPTION__(message.c_str());
    }

    action->end();

    std::shared_ptr<ISkyDeviceAction> newActionGuard(newAction);
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
    interface->sendData(messageBuildingBuffer, message.getMessageSize());
}

void SkyDevice::send(const ISignalPayloadMessage& message)
{
    ISignalPayloadMessage::MessagesBuilder builder(&message);
    while (builder.hasNext())
    {
        builder.getNext(messageBuildingBuffer);
        interface->sendData(messageBuildingBuffer, IMessage::SIGNAL_DATA_MESSAGE_SIZE);
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
