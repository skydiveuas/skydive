#include "IdleAction.hpp"

#include "OperatorEvent.hpp"

#include "ConnectAction.hpp"
#include "UpgradeAction.hpp"
#include "WhoAmIAction.hpp"

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

void IdleAction::handleUserEvent(const OperatorEvent& event)
{
    switch (event.getType())
    {
    case OperatorEvent::CONNECT:
    {
        const OperatorEventConnect& ev = reinterpret_cast<const OperatorEventConnect&>(event);
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

        default:
            except("Unexpected connection type");
        }
        break;
    }

    default:
        except("Unexpected user event received");
    }
}
