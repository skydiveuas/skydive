#include "RadioCheckAction.hpp"

#include "AppAction.hpp"

#include "OperatorEvent.hpp"

RadioCheckAction::RadioCheckAction(Listener* const _listener):
    ISkyDeviceAction(_listener)
{
    state = IDLE;
}

void RadioCheckAction::start(void)
{
    monitor->trace("Check radio procedure requested");
    state = INITIAL_COMMAND;
    listener->enableConnectionTimeoutTask(true);
    sendSignal(SignalData::CHECK_RADIO, SignalData::START);
}

IMessage::MessageType RadioCheckAction::getExpectedControlMessageType(void) const
{
    return IMessage::CONTROL_DATA;
}

bool RadioCheckAction::isActionDone(void) const
{
    return IDLE == state;
}

ISkyDeviceAction::Type RadioCheckAction::getType(void) const
{
    return RADIO_CHECK;
}

std::string RadioCheckAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    case CONTROLS_RECEPTION: return "CONTROLS_RECEPTION";
    case RUNNING: return "RUNNING";
    case BREAKING: return "BREAKING";
    default:
        __SKY_EXCEPTION__("RadioCheckAction::getStateName::Unexpected state");
    }
}

void RadioCheckAction::handleReception(const IMessage& message)
{
    switch (state)
    {
    case IDLE:
    case INITIAL_COMMAND:
        handleIdleReception(message);
        break;

    case CONTROLS_RECEPTION:
        if (handleSignalPayloadReception(message))
        {
            monitor->trace("Check radio started");
            monitor->notifyDeviceEvent(new UavEventReceived(message));
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CHECK_RADIO_STARTED));
            state = RUNNING;
        }
        break;

    case BREAKING:
    case RUNNING:
        if (IMessage::CONTROL_DATA == message.getMessageType())
        {
            monitor->notifyDeviceEvent(new UavEventReceived(message));
        }
        else
        {
            except("Unexpeted message received", message);
        }
        break;

    default:
        except("Message received in unexpected state", message);
    }
}

void RadioCheckAction::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Controls reception started");
            listener->enableConnectionTimeoutTask(false);
            initializeSignalPayloadReception(SignalData::CONTROL_SETTINGS);
            state = CONTROLS_RECEPTION;
            break;

        case SignalData::NOT_ALLOWED:
            monitor->trace("Check radio not allowed, breaking");
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CHECK_RADIO_NOT_ALLOWED));
            state = IDLE;
            listener->startAction(new AppAction(listener));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    case BREAKING:
        switch (parameter)
        {
        case SignalData::BREAK_ACK:
            monitor->trace("Check radio done");
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CHECK_RADIO_ENDED));
            state = IDLE;
            listener->startAction(new AppAction(listener));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}

void RadioCheckAction::handleUserEvent(const OperatorEvent& event)
{
    if (RUNNING == state)
    {
        switch (event.getType())
        {
        case OperatorEvent::RADIO_CHECK_DONE:
            state = BREAKING;
            listener->enableConnectionTimeoutTask(false);
            sendSignal(SignalData::CHECK_RADIO, SignalData::BREAK);
            break;

        default:
            except("Unexpected user uav event received");
        }
    }
    else
    {
        except("Uav event received at unexpected state");
    }
}

