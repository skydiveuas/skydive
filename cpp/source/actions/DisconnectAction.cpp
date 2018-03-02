#include "DisconnectAction.hpp"

#include "IdleAction.hpp"

#include "Exception.hpp"

DisconnectAction::DisconnectAction(Listener* const _listener):
    ICommAction(_listener)
{
    state = IDLE;
}

void DisconnectAction::start(void)
{
    monitor->trace("Disconnect procedure started");
    state = DISCONNECTING;
    sendSignal(SignalData::APP_LOOP, SignalData::BREAK);
}

bool DisconnectAction::isActionDone(void) const
{
    return IDLE == state;
}

ICommAction::Type DisconnectAction::getType(void) const
{
    return DISCONNECT;
}

std::string DisconnectAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case DISCONNECTING: return "DISCONNECTING";
    default:
        __RL_EXCEPTION__("AppAction::getStateName::Unexpected state");
    }
}

void DisconnectAction::handleReception(const IMessage& message)
{
    handleIdleReception(message);
}

void DisconnectAction::handleSignalReception(const Parameter parameter)
{
    if (DISCONNECTING == state && SignalData::BREAK_ACK == parameter)
    {
        monitor->trace("App. loop disconnected");
        state = IDLE;
        listener->startAction(new IdleAction(listener));
        listener->disconnectInterface();
        monitor->notifyUavEvent(new UavEvent(UavEvent::APPLICATION_LOOP_ENDED));
    }
}
