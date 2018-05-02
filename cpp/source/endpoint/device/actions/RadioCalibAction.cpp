#include "endpoint/device/actions/RadioCalibAction.hpp"

#include "endpoint/device/actions/AppAction.hpp"

#include "endpoint/device/PilotEvent.hpp"

RadioCalibAction::RadioCalibAction(Listener* const _listener):
    ISkyDeviceAction(_listener)
{
    state = IDLE;
}

void RadioCalibAction::start(void)
{
    current = 0;
    monitor->trace("Calibrate radio procedure requested");
    state = INITIAL_COMMAND;
    sendSignal(SignalData::CALIBRATE_RADIO, SignalData::START);
}

IMessage::MessageType RadioCalibAction::getExpectedControlMessageType(void) const
{
    return IMessage::CONTROL_DATA;
}

bool RadioCalibAction::isActionDone(void) const
{
    return IDLE == state;
}

ISkyDeviceAction::Type RadioCalibAction::getType(void) const
{
    return RADIO_CALIB;
}

std::string RadioCalibAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    case CALIBRATION_COMMAND: return "CALIBRATION_COMMAND";
    case BREAKING: return "BREAKING";
    case CHECK: return "CHECK";
    case CALIBRATION_RESPONSE: return "CALIBRATION_RESPONSE";
    case FINAL_COMMAND: return "FINAL_COMMAND";
    case CALIBRATION_RECEPTION: return "CALIBRATION_RECEPTION";
    default:
        __SKY_EXCEPTION__("RadioCalibAction::getStateName::Unexpected state");
    }
}

void RadioCalibAction::handleReception(const IMessage& message)
{
    switch (state)
    {
    case IDLE:
    case INITIAL_COMMAND:
        handleIdleReception(message);
        break;

    case FINAL_COMMAND:
    case CHECK:
        if (IMessage::CONTROL_DATA == message.getMessageType())
        {
            monitor->notifyDeviceEvent(new DeviceEventReceived(message));
        }
        else
        {
            except("Unexpeted message received", message);
        }
        break;

    case CALIBRATION_RECEPTION:
        if (IMessage::CALIBRATION_SETTINGS == message.getMessageType())
        {
            monitor->trace("Calibration settings received, radio calibration successfull");
            monitor->notifyDeviceEvent(new DeviceEventReceived(message));
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CALIBRATE_RADIO_ENDED));
            monitor->notifyDeviceEvent(new DeviceEventMessage(DeviceEventMessage::INFO,
                                                              "Radio receiver calibration successfull."));
            state = IDLE;
            listener->startAction(new AppAction(listener));
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

void RadioCalibAction::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Calibrate radio procedure started");
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CALIBRATE_RADIO_STARTED));
            state = CALIBRATION_COMMAND;
            break;

        case SignalData::NOT_ALLOWED:
            monitor->trace("Calibrate radio not allowed, breaking");
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CALIBRATE_RADIO_NOT_ALLOWED));
            state = IDLE;
            listener->startAction(new AppAction(listener));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    case CALIBRATION_RESPONSE:
        switch (parameter)
        {
        case SignalData::ACK:
            current++;
            monitor->trace("Calibrate radio response: ACK, chanel: " + std::to_string(current));
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CALIBRATE_RADIO_ACK));
            if (current >= 8)
            {
                monitor->trace("Switching to CHECK");
                state = CHECK;
            }
            else
            {
                state = CALIBRATION_COMMAND;
            }
            break;

        case SignalData::FAIL:
            monitor->trace("Calibrate radio response: FAIL");
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CALIBRATE_RADIO_FAIL));
            state = CALIBRATION_COMMAND;
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    case BREAKING:
        switch (parameter)
        {
        case SignalData::BREAK_ACK:
            monitor->trace("Radio calibration broken");
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CALIBRATE_RADIO_ENDED));
            state = IDLE;
            listener->startAction(new AppAction(listener));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    case FINAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Radio calibration final command ACK");
            state = CALIBRATION_RECEPTION;
            initializeSignalPayloadReception(SignalData::CALIBRATION_SETTINGS);
            break;

        case SignalData::BREAK_FAIL:
            monitor->trace("Radio calibration final command FAIL");
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CALIBRATE_RADIO_ENDED));
            monitor->notifyDeviceEvent(new DeviceEventMessage(DeviceEventMessage::WARNING,
                                                              "Error while safing data to internal memory."
                                                              "Calibration results discarded."));
            state = IDLE;
            listener->startAction(new AppAction(listener));
            break;

        case SignalData::BREAK_ACK:
            monitor->trace("Radio calibration final command BREAK_ACK");
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CALIBRATE_RADIO_ENDED));
            monitor->notifyDeviceEvent(new DeviceEventMessage(DeviceEventMessage::INFO,
                                                              "Calibration results discarded."));
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

void RadioCalibAction::handleUserEvent(const PilotEvent& event)
{
    switch (state)
    {
    case CALIBRATION_COMMAND:
        switch (event.getType())
        {
        case PilotEvent::RADIO_CALIBRATION_DONE:
            state = CALIBRATION_RESPONSE;
            sendSignal(SignalData::CALIBRATE_RADIO, SignalData::DONE);
            break;

        case PilotEvent::RADIO_CALIBRATION_SKIP:
            state = CALIBRATION_RESPONSE;
            sendSignal(SignalData::CALIBRATE_RADIO, SignalData::SKIP);
            break;

        case PilotEvent::RADIO_CALIBRATION_BREAK:
            state = BREAKING;
            sendSignal(SignalData::CALIBRATE_RADIO, SignalData::BREAK);
            break;

        default:
            except("Unexpected user uav event received");
        }
        break;

    case CHECK:
        switch (event.getType())
        {
        case PilotEvent::RADIO_CALIBRATION_DONE:
            state = FINAL_COMMAND;
            sendSignal(SignalData::CALIBRATE_RADIO, SignalData::BREAK);
            break;

        case PilotEvent::RADIO_CALIBRATION_SKIP:
            state = FINAL_COMMAND;
            sendSignal(SignalData::CALIBRATE_RADIO, SignalData::BREAK_FAIL);
            break;

        default:
            except("Unexpected user uav event received");
        }
        break;

    default:
        except("Uav event received at unexpected state");
    }
}


