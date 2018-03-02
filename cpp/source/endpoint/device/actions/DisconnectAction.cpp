#include "DisconnectAction.hpp"

#include "IdleAction.hpp"

#include "SkyException.hpp"

DisconnectAction::DisconnectAction(Listener* const _listener):
    ISkyDeviceAction(_listener)
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

ISkyDeviceAction::Type DisconnectAction::getType(void) const
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
        __SKY_EXCEPTION__("AppAction::getStateName::Unexpected state");
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
        monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::APPLICATION_LOOP_ENDED));
    }
}
