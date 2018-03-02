#include "DownloadSignalPaylod.hpp"

#include "AppAction.hpp"

DownloadSignalPaylod::DownloadSignalPaylod(Listener* const _listener, const SignalData::Command _type):
    ISkyDeviceAction(_listener),
    type(_type),
    downloadCommand(getDownloadCommand())
{
    state = IDLE;
}

void DownloadSignalPaylod::start(void)
{
    monitor->trace("Signal payload download procedure requested");
    state = INITIAL_COMMAND;
    sendSignal(downloadCommand, SignalData::START);
}

bool DownloadSignalPaylod::isActionDone(void) const
{
    return IDLE == state;
}

ISkyDeviceAction::Type DownloadSignalPaylod::getType(void) const
{
    return DOWNLOAD_SIGNAL_PAYLOAD;
}

std::string DownloadSignalPaylod::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    case DATA_RECEPTION: return "DATA_RECEPTION";
    default:
        __SKY_EXCEPTION__("DownloadSignalPaylo__SKY_EXCEPTION__:Unexpected state");
    }
}

void DownloadSignalPaylod::handleReception(const IMessage& message)
{
    switch (state)
    {
    case IDLE:
    case INITIAL_COMMAND:
        handleIdleReception(message);
        break;

    case DATA_RECEPTION:
        if (handleSignalPayloadReception(message))
        {
            state = IDLE;
            listener->startAction(new AppAction(listener));
            monitor->notifyDeviceEvent(new UavEventReceived(message));
        }
        break;

    default:
        except("Message received in unexpected state", message);
    }
}

void DownloadSignalPaylod::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Signal payload download procedure started");
            initializeSignalPayloadReception(type);
            state = DATA_RECEPTION;
            break;

        case SignalData::FAIL:
            monitor->notifyDeviceEvent(new DeviceEvent(getMonitorFailEvent()));
            state = IDLE;
            listener->startAction(new AppAction(listener));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}

SignalData::Command DownloadSignalPaylod::getDownloadCommand(void) const
{
    switch (type)
    {
    case SignalData::CONTROL_SETTINGS: return SignalData::DOWNLOAD_SETTINGS;
    case SignalData::ROUTE_CONTAINER: return SignalData::DOWNLOAD_ROUTE;
    default:
        __SKY_EXCEPTION__("DownloadSignalPaylod::getDownloadCommand::Unexpected type");
    }
}

DeviceEvent::Type DownloadSignalPaylod::getMonitorFailEvent(void) const
{
    switch (type)
    {
    case SignalData::CONTROL_SETTINGS: return DeviceEvent::CONTROLS_DOWNLOAD_FAIL;
    case SignalData::ROUTE_CONTAINER: return DeviceEvent::ROUTE_DOWNLOAD_FAIL;
    default:
        __SKY_EXCEPTION__("DownloadSignalPaylod::getMonitorFailEvent::Unexpected type");
    }
}
