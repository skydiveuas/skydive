#include "AppAction.hpp"

#include "Exception.hpp"

#include "UserUavEvent.hpp"

#include "UpgradeAction.hpp"
#include "DisconnectAction.hpp"
#include "FlightInitializationAction.hpp"
#include "FlightAction.hpp"
#include "AccelCalibAction.hpp"
#include "MagnetCalibAction.hpp"
#include "UploadSignalPayload.hpp"
#include "DownloadSignalPaylod.hpp"
#include "SensorsLoggerAction.hpp"
#include "RadioCheckAction.hpp"
#include "RadioCalibAction.hpp"
#include "ResetAction.hpp"
#include "EscCalibAction.hpp"

#include <functional>

AppAction::AppAction(Listener* const _listener):
    ICommAction(_listener)
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

ICommAction::Type AppAction::getType(void) const
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
        __RL_EXCEPTION__("AppAction::getStateName::Unexpected state");
    }
}

void AppAction::handleReception(const IMessage& message)
{
    switch (state)
    {
    case APP_LOOP:
        if (IMessage::DEBUG_DATA == message.getMessageType())
        {
            monitor->notifyUavEvent(new UavEventReceived(message));
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

void AppAction::handleUserEvent(const UserUavEvent& event)
{
    switch (event.getType())
    {
    case UserUavEvent::ACTION:
        handleUserUavEventAction(reinterpret_cast<const UserUavEventAction&>(event));
        break;

    case UserUavEvent::UPLOAD:
        handleUserUavEventUpload(reinterpret_cast<const UserUavEventUpload&>(event));
        break;

    case UserUavEvent::DOWNLOAD:
        handleUserUavEventDownload(reinterpret_cast<const UserUavEventDownload&>(event));
        break;

    default:
        except("Unexpected user event received");
    }
}

void AppAction::handleUserUavEventAction(const UserUavEventAction& event)
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

void AppAction::handleUserUavEventUpload(const UserUavEventUpload& event)
{
    listener->startAction(new UploadSignalPayload(listener, *event.getData().clone()));
}

void AppAction::handleUserUavEventDownload(const UserUavEventDownload& event)
{
    listener->startAction(new DownloadSignalPaylod(listener, event.getDataType()));
}
