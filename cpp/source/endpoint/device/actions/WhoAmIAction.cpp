#include "WhoAmIAction.hpp"

#include "IdleAction.hpp"
#include "AppAction.hpp"

WhoAmIAction::WhoAmIAction(Listener* const _listener):
    ISkyDeviceAction(_listener)
{
    state = IDLE;
}

void WhoAmIAction::start(void)
{
    monitor->trace("Who am I procedure requested");
    state = INITIAL_COMMAND;
    sendSignal(SignalData::WHO_AM_I_VALUE, SignalData::START);
}

bool WhoAmIAction::isActionDone(void) const
{
    return IDLE == state;
}

ISkyDeviceAction::Type WhoAmIAction::getType(void) const
{
    return WHO_AM_I;
}

std::string WhoAmIAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    default:
        __SKY_EXCEPTION__("WhoAmIAction::get__SKY_EXCEPTION__ected state");
    }
}

void WhoAmIAction::handleReception(const IMessage& message)
{
    handleIdleReception(message);
}

void WhoAmIAction::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        state = IDLE;
        listener->startAction(new IdleAction(listener));
        monitor->notifyDeviceEvent(new UavEventWhoAmI(static_cast<CalibrationSettings::BoardType>(parameter)));
        monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::APPLICATION_LOOP_TERMINATED));
        listener->disconnectInterface();
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}
