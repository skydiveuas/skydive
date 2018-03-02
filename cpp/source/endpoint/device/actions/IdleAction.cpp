#include "IdleAction.hpp"

#include "UserUavEvent.hpp"

#include "ConnectAction.hpp"
#include "UpgradeAction.hpp"
#include "WhoAmIAction.hpp"

IdleAction::IdleAction(Listener* const _listener):
    ICommAction(_listener)
{
}

void IdleAction::start(void)
{
}

bool IdleAction::isActionDone(void) const
{
    return true;
}

ICommAction::Type IdleAction::getType(void) const
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

void IdleAction::handleUserEvent(const UserUavEvent& event)
{
    switch (event.getType())
    {
    case UserUavEvent::CONNECT:
    {
        const UserUavEventConnect& ev = reinterpret_cast<const UserUavEventConnect&>(event);
        switch (ev.getConnectionType())
        {
        case ICommAction::APP:
            listener->startAction(new ConnectAction(listener), false);
            listener->connectInterface(ev.getCommInnterface());
            break;

        case ICommAction::UPGRADE:
            listener->startAction(new UpgradeAction(listener), false);
            listener->connectInterface(ev.getCommInnterface());
            break;

        case ICommAction::WHO_AM_I:
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
