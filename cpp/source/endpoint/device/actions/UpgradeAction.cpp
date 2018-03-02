#include "UpgradeAction.hpp"

#include "IdleAction.hpp"
#include "AppAction.hpp"

UpgradeAction::UpgradeAction(Listener* const _listener):
    ISkyDeviceAction(_listener)
{
    state = IDLE;
}

void UpgradeAction::start(void)
{
    monitor->trace("Upgrade procedure requested");
    state = INITIAL_COMMAND;
    sendSignal(SignalData::SOFTWARE_UPGRADE, SignalData::START);
}

bool UpgradeAction::isActionDone(void) const
{
    return IDLE == state;
}

ISkyDeviceAction::Type UpgradeAction::getType(void) const
{
    return UPGRADE;
}

std::string UpgradeAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    default:
        __SKY_EXCEPTION__("UpgradeAction::ge__SKY_EXCEPTION__pected state");
    }
}

void UpgradeAction::handleReception(const IMessage& message)
{
    handleIdleReception(message);
}

void UpgradeAction::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            state = IDLE;
            listener->startAction(new IdleAction(listener));
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::UPGRADE_STARTED));
            monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::APPLICATION_LOOP_TERMINATED));
            listener->disconnectInterface();
            break;

        case SignalData::NOT_ALLOWED:
            state = IDLE;
            listener->startAction(new AppAction(listener));
            monitor->notifyDeviceEvent(new UavEventMessage(UavEventMessage::WARNING,
                                                        "Board refused upgrade command, can not"
                                                        " be done over wireless interface."));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}

