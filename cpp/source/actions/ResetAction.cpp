#include "ResetAction.hpp"

#include "IdleAction.hpp"

ResetAction::ResetAction(Listener* const _listener):
    ICommAction(_listener)
{
    state = IDLE;
}

void ResetAction::start(void)
{
    monitor->trace("Board reset procedure requested");
    state = INITIAL_COMMAND;
    sendSignal(SignalData::SYSTEM_RESET, SignalData::START);
}

bool ResetAction::isActionDone(void) const
{
    return IDLE == state;
}

ICommAction::Type ResetAction::getType(void) const
{
    return RESET;
}

std::string ResetAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    default:
        __RL_EXCEPTION__("ResetAction::getStateName::Unexpected state");
    }
}

void ResetAction::handleReception(const IMessage& message)
{
    handleIdleReception(message);
}

void ResetAction::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Board reset procedure started");
            state = IDLE;
            listener->startAction(new IdleAction(listener));
            listener->disconnectInterface();
            monitor->notifyUavEvent(new UavEvent(UavEvent::APPLICATION_LOOP_TERMINATED));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}

