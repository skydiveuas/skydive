#include "endpoint/device/actions/IdleAction.hpp"

#include "endpoint/device/PilotEvent.hpp"

#include "endpoint/device/actions/ConnectAction.hpp"
#include "endpoint/device/actions/UpgradeAction.hpp"
#include "endpoint/device/actions/WhoAmIAction.hpp"

#include "endpoint/device/actions/FlightAction.hpp"
#include "communication/ControlSettings.hpp"
#include "communication/RouteContainer.hpp"

IdleAction::IdleAction(Listener* const _listener):
    ISkyDeviceAction(_listener)
{
}

void IdleAction::start(void)
{
}

bool IdleAction::isActionDone(void) const
{
    return true;
}

ISkyDeviceAction::Type IdleAction::getType(void) const
{
    return IDLE_ACTION;
}

std::string IdleAction::getStateName(void) const
{
    return "IDLE";
}

void IdleAction::handleReception(const IMessage& message)
{
    except("Unexpected message received", message);
}

void IdleAction::handleUserEvent(const PilotEvent& event)
{
    switch (event.getType())
    {
    case PilotEvent::CONNECT:
    {
        const PilotEventConnect& ev = reinterpret_cast<const PilotEventConnect&>(event);
        switch (ev.getConnectionType())
        {
        case ISkyDeviceAction::APP:
            listener->startAction(new ConnectAction(listener), false);
            listener->connectInterface(ev.getCommInnterface());
            break;

        case ISkyDeviceAction::UPGRADE:
            listener->startAction(new UpgradeAction(listener), false);
            listener->connectInterface(ev.getCommInnterface());
            break;

        case ISkyDeviceAction::WHO_AM_I:
            listener->startAction(new WhoAmIAction(listener), false);
            listener->connectInterface(ev.getCommInnterface());
            break;

        case ISkyDeviceAction::DIRECT_FLIGHT:
            handleDirectFlightCommand(ev);
            break;

        default:
            except("Unexpected connection type");
        }
        break;
    }

    default:
        except("Unexpected user event received");
    }
}

void IdleAction::handleDirectFlightCommand(const PilotEventConnect& ev) {
    // Configuration data needs to be created and filled with predefined values,
    // as it is not possible to fetch it from Device in direcct mode.
    // It could be deliverd in operation context, but keep it simple for now.
    CalibrationSettings calib = CalibrationSettings::createDefault();
    ControlSettings control = ControlSettings::createDefault();
    control.uavType = ControlSettings::QUADROCOPTER_X;
    control.initialSolverMode = ControlData::ANGLE;
    control.manualThrottleMode = ControlSettings::DYNAMIC;

    monitor->notifyDeviceEvent(new DeviceEventReceived(*new CalibrationSettings(calib)));
    monitor->notifyDeviceEvent(new DeviceEventReceived(*new ControlSettings(control)));
    monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::VIA_ROUTE_NOT_ALLOWED));
    listener->startAction(new FlightAction(listener, monitor->getControlDataSendingFreq()), false);
    listener->connectInterface(ev.getCommInnterface());
}
