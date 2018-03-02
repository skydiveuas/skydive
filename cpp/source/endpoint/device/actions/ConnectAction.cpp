#include "ConnectAction.hpp"

#include "SkyException.hpp"

#include "AppAction.hpp"

#include "CalibrationSettings.hpp"

ConnectAction::ConnectAction(Listener* const _listener):
    ISkyDeviceAction(_listener)
{
    state = IDLE;
    wasNonStatic = false;
}

void ConnectAction::start(void)
{
    monitor->trace("ConnectAction::start");
    state = INITIAL_COMMAND;
    sendSignal(SignalData::START_CMD, SignalData::START);
}

bool ConnectAction::isActionDone(void) const
{
    return state == IDLE;
}

ISkyDeviceAction::Type ConnectAction::getType(void) const
{
    return CONNECT;
}

std::string ConnectAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    case CALIBRATION: return "CALIBRATION";
    case CALIBRATION_RECEPTION: return "CALIBRATION_RECEPTION";
    case FINAL_COMMAND: return "FINAL_COMMAND";
    default:
        __SKY_EXCEPTION__("ConnectAction::getStateName::Unexpected state");
    }
}

void ConnectAction::handleReception(const IMessage& message)
{
    switch (state)
    {
    case CALIBRATION_RECEPTION:
        if (handleSignalPayloadReception(message))
        {
            state = FINAL_COMMAND;
            listener->send(SignalData(SignalData::APP_LOOP, SignalData::START));
            startSignalTimeout(SignalData::APP_LOOP);
            monitor->notifyDeviceEvent(new UavEventReceived(message));
        }
        break;

    default:
        except("Message received in unexpected state", message);
    }
}

void ConnectAction::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Initial command successfull");
            startSignalTimeout(SignalData::CALIBRATION_SETTINGS);
            state = CALIBRATION;
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    case CALIBRATION:
        switch (parameter)
        {
        case SignalData::READY:
            monitor->trace("Calibration done");
            initializeSignalPayloadReception(SignalData::CALIBRATION_SETTINGS);
            state = CALIBRATION_RECEPTION;
            break;

        case SignalData::NON_STATIC:
            monitor->trace("Calibration non-static");
            startSignalTimeout(SignalData::CALIBRATION_SETTINGS);
            if (!wasNonStatic)
            {
                wasNonStatic = true;
                monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::CALIBRATION_NON_STATIC));
            }
            break;

        default:
            except("Unexpected signal parameter received", parameter);
        }
        break;

    case FINAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("App loop ready");
            state = IDLE;
            listener->startAction(new AppAction(listener));
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::APPLICATION_LOOP_STARTED));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}
