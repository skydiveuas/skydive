#include "SkyDiveUav.hpp"

#include "Exception.hpp"

#include "UserUavEvent.hpp"

#include "IdleAction.hpp"

#include <stdlib.h>
#include <functional>

SkyDiveUav::SkyDiveUav(ISkyDiveMonitor* const _monitor,
                               const double _pingFreq,
                               const double _controlFreq,
                               const double _connectionTimeout) :
    monitor(_monitor),
    pingFreq(_pingFreq),
    controlFreq(_controlFreq),
    connectionTimeoutFreq(1 / _connectionTimeout),
    pingTimer(monitor->createTimer(std::bind(&SkyDiveUav::pingTimerHandler, this))),
    connetionTimer(monitor->createTimer(std::bind(&SkyDiveUav::connectionTimerHandler, this)))
{
    action = std::make_shared<IdleAction>(this);

    std::srand(std::time(0));
}

void SkyDiveUav::pushUserUavEvent(const UserUavEvent* const userUavEvent)
{
    pushUserUavEvent(std::unique_ptr<const UserUavEvent>(userUavEvent));
}

void SkyDiveUav::pushUserUavEvent(std::unique_ptr<const UserUavEvent> userUavEvent)
{
    monitor->trace("Handling user event: " + userUavEvent->toString() + " at: " + action->getName());
    notifyUserUavEvent(action, userUavEvent.get());
}

ICommAction::Type SkyDiveUav::getState(void) const
{
    if (nullptr == action)
    {
        return ICommAction::IDLE_ACTION;
    }
    else
    {
        return action->getType();
    }
}

void SkyDiveUav::notifyUserUavEvent(std::shared_ptr<ICommAction> actionLock,
                                        const UserUavEvent* const userUavEvent)
{
    actionLock->handleUserEvent(*userUavEvent);
}

void SkyDiveUav::notifyReception(std::shared_ptr<ICommAction> actionLock,
                                     const IMessage* const message)
{
//    monitor->trace("HandleReception with " + actionLock->getName() +
//                   " msg: " + message->getMessageName());

    receptionFeed = true;
    if (connectionLost)
    {
        connectionLost = false;
        monitor->notifyUavEvent(new UavEvent(UavEvent::CONNECTION_RECOVERED));
    }

    try
    {
        actionLock->baseHandleReception(std::unique_ptr<const IMessage>(message));
    }
    catch (Exception e)
    {
        handleError(e.message());
    }
}

void SkyDiveUav::handleError(const std::string& message)
{
    monitor->trace("SkyDiveUav::handleError: " + message);
    enablePingTask(false);
    enableConnectionTimeoutTask(false);
    std::shared_ptr<ICommAction> newAct = std::make_shared<IdleAction>(this);
    action.swap(newAct);
    monitor->notifyUavEvent(new UavEventMessage(UavEventMessage::ERROR, message));
    interface->disconnect();
}

void SkyDiveUav::pingTimerHandler(void)
{
    sentPingValue = std::rand();
    sentPingTime = clock();
    send(SignalData(SignalData::PING_VALUE, sentPingValue));
}

void SkyDiveUav::handlePong(const SignalData& signalData) const
{
    if (signalData.getParameterValue() == sentPingValue)
    {
        unsigned ping = (unsigned)(((clock() - sentPingTime) / 2.0f) + 0.5f);
        monitor->notifyUavEvent(new UavEventConnectionStatus(
                                    ping,
                                    dispatcher.getSucessfullReceptions(),
                                    dispatcher.getFailedReceptions()));
    }
}

void SkyDiveUav::connectionTimerHandler(void)
{
    if (false == receptionFeed)
    {
        // connection timeout ocured
        if (false == connectionLost)
        {
            // first time
            connectionLost = true;
            monitor->notifyUavEvent(new UavEvent(UavEvent::CONNECTION_LOST));
        }
    }
    receptionFeed = false;
}

void SkyDiveUav::onConnected()
{
    monitor->trace("SkyDiveUav::onConnected");
    action->start();
}

void SkyDiveUav::onDisconnected()
{
    monitor->trace("SkyDiveUav::onDisconnected");
}

void SkyDiveUav::onError(const std::string& message)
{
    handleError(message);
}

void SkyDiveUav::onDataReceived(const unsigned char* data, const unsigned dataSize)
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

ISkyDiveMonitor* SkyDiveUav::getMonitor(void)
{
    return monitor;
}

void SkyDiveUav::startAction(ICommAction* newAction, bool immediateStart)
{
    monitor->trace("Starting new action: " + newAction->getName());

    if (false == action->isActionDone())
    {
        std::string message = "Previous action (" + action->getName() +
                ") not done, when performing: " + newAction->getName() + ".";
        __RL_EXCEPTION__(message.c_str());
    }

    action->end();

    std::shared_ptr<ICommAction> newActionShared(newAction);
    action.swap(newActionShared);

    if (immediateStart)
    {
        action->start();
    }
}

void SkyDiveUav::onPongReception(const SignalData& pong)
{
    handlePong(pong);
}

void SkyDiveUav::send(const IMessage& message)
{
    message.serializeMessage(messageBuildingBuffer);
    interface->sendData(messageBuildingBuffer, message.getMessageSize());
}

void SkyDiveUav::send(const ISignalPayloadMessage& message)
{
    ISignalPayloadMessage::MessagesBuilder builder(&message);
    while (builder.hasNext())
    {
        builder.getNext(messageBuildingBuffer);
        interface->sendData(messageBuildingBuffer, IMessage::SIGNAL_DATA_MESSAGE_SIZE);
    }
}

void SkyDiveUav::enablePingTask(bool enable)
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

void SkyDiveUav::enableConnectionTimeoutTask(bool enable)
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

void SkyDiveUav::connectInterface(IAppCommInterface* _interface)
{
    interface = _interface;
    interface->setListener(this);
    interface->connect();
}

void SkyDiveUav::disconnectInterface(void)
{
    enablePingTask(false);
    enableConnectionTimeoutTask(false);
    interface->disconnect();
}
