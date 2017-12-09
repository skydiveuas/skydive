#include "WhoAmIAction.hpp"

#include "IdleAction.hpp"
#include "AppAction.hpp"

WhoAmIAction::WhoAmIAction(Listener* const _listener):
    ICommAction(_listener)
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

ICommAction::Type WhoAmIAction::getType(void) const
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
        __RL_EXCEPTION__("WhoAmIAction::getStateName::Unexpected state");
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
        monitor->notifyUavEvent(new UavEventWhoAmI(static_cast<CalibrationSettings::BoardType>(parameter)));
        monitor->notifyUavEvent(new UavEvent(UavEvent::APPLICATION_LOOP_TERMINATED));
        listener->disconnectInterface();
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}
