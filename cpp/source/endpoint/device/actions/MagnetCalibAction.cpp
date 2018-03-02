#include "MagnetCalibAction.hpp"

#include "AppAction.hpp"

#include "OperatorEvent.hpp"

MagnetCalibAction::MagnetCalibAction(Listener* const _listener):
    ISkyDeviceAction(_listener)
{
    state = IDLE;
}

void MagnetCalibAction::start(void)
{
    monitor->trace("Magnetometer calibration procedure requested");
    state = INITIAL_COMMAND;
    sendSignal(SignalData::CALIBRATE_MAGNET, SignalData::START);
}

bool MagnetCalibAction::isActionDone(void) const
{
    return IDLE == state;
}

ISkyDeviceAction::Type MagnetCalibAction::getType(void) const
{
    return MAGNET_CALIB;
}

std::string MagnetCalibAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    case CALIBRATION: return "CALIBRATION";
    case CALIBRATION_RESULT: return "CALIBRATION_RESULT";
    case CALIBRATION_SKIP_RESULT: return "CALIBRATION_SKIP_RESULT";
    case CALIBRATION_RECEPTION: return "IDLE";
    default:
        __SKY_EXCEPTION__("MagnetCalibAction::getStateName::Unexpected state");
    }
}

void MagnetCalibAction::handleReception(const IMessage& message)
{
    switch (state)
    {
    case IDLE:
    case INITIAL_COMMAND:
    case CALIBRATION_RESULT:
    case CALIBRATION_SKIP_RESULT:
        handleIdleReception(message);
        break;

    case CALIBRATION_RECEPTION:
        if (handleSignalPayloadReception(message))
        {
            monitor->notifyDeviceEvent(new UavEventReceived(message));
            state = IDLE;
            listener->startAction(new AppAction(listener));
        }
        break;

    default:
        except("Message received in unexpected state", message);
    }
}

void MagnetCalibAction::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Magnetometer calibration procedure started");
            listener->enableConnectionTimeoutTask(false);
            state = CALIBRATION;
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CALIBRATE_MAGNET_STARTED));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    case CALIBRATION_RESULT:
        switch (parameter)
        {
        case SignalData::DONE:
            monitor->trace("Magnetometer calibration done, receiving settings");
            initializeSignalPayloadReception(SignalData::CALIBRATION_SETTINGS);
            state = CALIBRATION_RECEPTION;
            break;

        case SignalData::FAIL:
            monitor->notifyDeviceEvent(new UavEventMessage(UavEventMessage::WARNING,
                                                        "Error during solving magnetometer calibration!\n"
                                                        "Bad data acquired."));
            state = IDLE;
            listener->startAction(new AppAction(listener));
            break;

        default:
            except("Unexpected signal parameter received", parameter);
        }
        break;

    case CALIBRATION_SKIP_RESULT:
        switch (parameter)
        {
        case SignalData::ACK:
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


void MagnetCalibAction::handleUserEvent(const OperatorEvent& event)
{
    if (CALIBRATION == state)
    {
        switch (event.getType())
        {
        case OperatorEvent::MAGNET_CALIB_DONE:
            state = CALIBRATION_RESULT;
            sendSignal(SignalData::CALIBRATE_MAGNET, SignalData::DONE);
            break;

        case OperatorEvent::MAGNET_CALIB_ABORT:
            state = CALIBRATION_SKIP_RESULT;
            sendSignal(SignalData::CALIBRATE_MAGNET, SignalData::SKIP);
            break;

        default:
            except("Unexpected user uav event received");
        }
        listener->enableConnectionTimeoutTask(true);
    }
    else
    {
        except("User uav event received at unexpected state");
    }
}
