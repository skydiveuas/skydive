#include "endpoint/device/actions/AppAction.hpp"

#include "endpoint/device/PilotEvent.hpp"

#include "endpoint/device/actions/UpgradeAction.hpp"
#include "endpoint/device/actions/DisconnectAction.hpp"
#include "endpoint/device/actions/FlightInitializationAction.hpp"
#include "endpoint/device/actions/FlightAction.hpp"
#include "endpoint/device/actions/AccelCalibAction.hpp"
#include "endpoint/device/actions/MagnetCalibAction.hpp"
#include "endpoint/device/actions/UploadSignalPayload.hpp"
#include "endpoint/device/actions/DownloadSignalPaylod.hpp"
#include "endpoint/device/actions/SensorsLoggerAction.hpp"
#include "endpoint/device/actions/RadioCheckAction.hpp"
#include "endpoint/device/actions/RadioCalibAction.hpp"
#include "endpoint/device/actions/ResetAction.hpp"
#include "endpoint/device/actions/EscCalibAction.hpp"

#include <functional>
#include <stdexcept>

AppAction::AppAction(Listener* const _listener):
    ISkyDeviceAction(_listener)
{
    state = IDLE;
}

void AppAction::start(void)
{
    state = APP_LOOP;
    listener->enablePingTask(true);
    listener->enableConnectionTimeoutTask(true);
}

void AppAction::end(void)
{
    listener->enablePingTask(false);
    listener->enableConnectionTimeoutTask(false);
}

bool AppAction::isActionDone(void) const
{
    return state == IDLE || state == APP_LOOP;
}

ISkyDeviceAction::Type AppAction::getType(void) const
{
    return APP;
}

std::string AppAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case APP_LOOP: return "APP_LOOP";
    default:
        throw std::runtime_error("AppAction::getStateName::Unexpected state");
    }
}

void AppAction::handleReception(const IMessage& message)
{
    switch (state)
    {
    case APP_LOOP:
        if (IMessage::DEBUG_DATA == message.getMessageType())
        {
            monitor->notifyDeviceEvent(new DeviceEventReceived(message));
        }
        else if (isPingMessage(message))
        {
            listener->onPongReception(reinterpret_cast<const SignalData&>(message));
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

void AppAction::handleUserEvent(const PilotEvent& event)
{
    switch (event.getType())
    {
    case PilotEvent::ACTION:
        handleUserUavEventAction(reinterpret_cast<const PilotEventAction&>(event));
        break;

    case PilotEvent::UPLOAD:
        handleUserUavEventUpload(reinterpret_cast<const PilotEventUpload&>(event));
        break;

    case PilotEvent::DOWNLOAD:
        handleUserUavEventDownload(reinterpret_cast<const PilotEventDownload&>(event));
        break;

    default:
        except("Unexpected user event received");
    }
}

void AppAction::handleUserUavEventAction(const PilotEventAction& event)
{
    switch (event.getAction())
    {
    case UPGRADE:
        listener->startAction(new UpgradeAction(listener));
        break;

    case DISCONNECT:
        listener->startAction(new DisconnectAction(listener));
        break;

    case FLIGHT_INITIALIZATION:
        listener->startAction(new FlightInitializationAction(listener));
        break;

    case ACCEL_CALIB:
        listener->startAction(new AccelCalibAction(listener));
        break;

    case MAGNET_CALIB:
        listener->startAction(new MagnetCalibAction(listener));
        break;

    case SENSORS_LOGGER:
        listener->startAction(new SensorsLoggerAction(listener));
        break;

    case RADIO_CHECK:
        listener->startAction(new RadioCheckAction(listener));
        break;

    case RADIO_CALIB:
        listener->startAction(new RadioCalibAction(listener));
        break;

    case ESC_CALIB:
        listener->startAction(new EscCalibAction(listener));
        break;

    case RESET:
        listener->startAction(new ResetAction(listener));
        break;

    default:
        except("Unexpected action to start");
    }
}

void AppAction::handleUserUavEventUpload(const PilotEventUpload& event)
{
    listener->startAction(new UploadSignalPayload(listener, *event.getData().clone()));
}

void AppAction::handleUserUavEventDownload(const PilotEventDownload& event)
{
    listener->startAction(new DownloadSignalPaylod(listener, event.getDataType()));
}
