#include "endpoint/device/actions/AccelCalibAction.hpp"

#include "endpoint/device/actions/AppAction.hpp"

AccelCalibAction::AccelCalibAction(Listener* const _listener):
    ISkyDeviceAction(_listener)
{
    state = IDLE;
}

void AccelCalibAction::start(void)
{
    monitor->trace("Accelerometer calibration procedure requested");
    state = INITIAL_COMMAND;
    sendSignal(SignalData::CALIBRATE_ACCEL, SignalData::START);
}

bool AccelCalibAction::isActionDone(void) const
{
    return IDLE == state;
}

ISkyDeviceAction::Type AccelCalibAction::getType(void) const
{
    return ACCEL_CALIB;
}

std::string AccelCalibAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    case CALIBRATION: return "CALIBRATION";
    case CALIBRATION_RECEPTION: return "CALIBRATION_RECEPTION";
    default:
        throw std::runtime_error("AccelCalibAction::throw std::runtime_errorexpected state");
    }
}

void AccelCalibAction::handleReception(const IMessage& message)
{
    switch (state)
    {
    case IDLE:
    case INITIAL_COMMAND:
        handleIdleReception(message);
        break;

    case CALIBRATION_RECEPTION:
        if (handleSignalPayloadReception(message))
        {
            monitor->notifyDeviceEvent(new DeviceEventReceived(message));
            state = IDLE;
            listener->startAction(new AppAction(listener));
        }
        break;

    default:
        except("Message received in unexpected state", message);
    }
}

void AccelCalibAction::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Accelerometer calibration procedure started");
            startSignalTimeout(SignalData::CALIBRATE_ACCEL, 3000);
            state = CALIBRATION;
            break;

        default:
            except("Unexpected signal parameter message received");
        }
        break;

    case CALIBRATION:
        switch (parameter)
        {
        case SignalData::DONE:
            monitor->trace("Accelerometer calibration done");
            initializeSignalPayloadReception(SignalData::CALIBRATION_SETTINGS);
            state = CALIBRATION_RECEPTION;
            break;

        case SignalData::NON_STATIC:
            monitor->notifyDeviceEvent(new DeviceEventMessage(DeviceEventMessage::WARNING,
                                                        "Accelerometer calibration aborted.\n"
                                                        "Was preformed in non-static conditions."));
            state = IDLE;
            listener->startAction(new AppAction(listener));
            break;

        default:
            except("Unexpected signal parameter received");
        }
        break;

    default:
        except("Signal parameter in unexpected state");
    }
}
