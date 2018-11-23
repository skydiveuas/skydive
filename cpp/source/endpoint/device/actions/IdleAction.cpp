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
            monitor->notifyDeviceEvent(new DeviceEventReceived(*new CalibrationSettings(CalibrationSettings::createDefault())));
            monitor->notifyDeviceEvent(new DeviceEventReceived(*new ControlSettings(ControlSettings::createDefault())));
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::VIA_ROUTE_NOT_ALLOWED));
            listener->startAction(new FlightAction(listener, monitor->getControlDataSendingFreq()), false);
            listener->connectInterface(ev.getCommInnterface());
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
