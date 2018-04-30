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
    case BOARD_VERSION: return "BOARD_VERSION";
    default:
        __SKY_EXCEPTION__("UpgradeAction::unexpected state");
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
            state = BOARD_VERSION;
            startSignalTimeout(SignalData::WHO_AM_I_VALUE);
            break;

        case SignalData::NOT_ALLOWED:
            state = IDLE;
            listener->startAction(new AppAction(listener));
            monitor->notifyDeviceEvent(new DeviceEventMessage(DeviceEventMessage::WARNING,
                                                              "Board refused upgrade command, cannot"
                                                              " be done over wireless interface."));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    case BOARD_VERSION:
        state = IDLE;
        listener->startAction(new IdleAction(listener));
        monitor->notifyDeviceEvent(new DeviceEventUpgradeStarted(static_cast<CalibrationSettings::BoardType>(parameter)));
        listener->disconnectInterface();
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}

