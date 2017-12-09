#include "SensorsLoggerAction.hpp"

#include "AppAction.hpp"

#include "UserUavEvent.hpp"

SensorsLoggerAction::SensorsLoggerAction(Listener* const _listener):
    ICommAction(_listener)
{
    state = IDLE;
}

void SensorsLoggerAction::start(void)
{
    monitor->trace("Sensors looger initialization procedure started");
    listener->enableConnectionTimeoutTask(true);
    state = INITIAL_COMMAND;
    sendSignal(SignalData::SENSORS_LOGGER, SignalData::START);
}

bool SensorsLoggerAction::isActionDone(void) const
{
    return IDLE == state;
}

IMessage::MessageType SensorsLoggerAction::getExpectedControlMessageType(void) const
{
    return IMessage::SENSORS_DATA;
}

ICommAction::Type SensorsLoggerAction::getType(void) const
{
    return SENSORS_LOGGER;
}

std::string SensorsLoggerAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    case LOGGING: return "LOGGING";
    case BREAKING: return "BREAKING";
    default:
        __RL_EXCEPTION__("SensorsLoggerAction::getStateName::Unexpected state");
    }
}

void SensorsLoggerAction::handleReception(const IMessage& message)
{
    switch (state)
    {
    case IDLE:
    case INITIAL_COMMAND:
    case BREAKING:
        handleIdleReception(message);
        break;

    case LOGGING:
        if (IMessage::SENSORS_DATA == message.getMessageType())
        {
            monitor->notifyUavEvent(new UavEventReceived(message));
        }
        else
        {
            except("Unexpected message received", message);
        }
        break;

    default:
        except("Message received in unexpected state", message);
    }
}

void SensorsLoggerAction::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            state = LOGGING;
            monitor->notifyUavEvent(new UavEvent(UavEvent::SENSORS_LOGGER_STARTED));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    case BREAKING:
        switch (parameter)
        {
        case SignalData::BREAK_ACK:
            state = IDLE;
            listener->startAction(new AppAction(listener));
            monitor->notifyUavEvent(new UavEvent(UavEvent::SENSORS_LOGGER_ENDED));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}

void SensorsLoggerAction::handleUserEvent(const UserUavEvent& event)
{
    if (LOGGING == state && UserUavEvent::BREAK_SENSORS_LOGGER == event.getType())
    {
        state = BREAKING;
        listener->enableConnectionTimeoutTask(false);
        sendSignal(SignalData::SENSORS_LOGGER, SignalData::BREAK);
    }
    else
    {
        except("User event received in unexpected state", event);
    }
}
