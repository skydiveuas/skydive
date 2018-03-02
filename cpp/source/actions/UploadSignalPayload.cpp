#include "UploadSignalPayload.hpp"

#include "AppAction.hpp"

UploadSignalPayload::UploadSignalPayload(Listener* const _listener,  const ISignalPayloadMessage& _data):
    ICommAction(_listener),
    data(_data),
    command(getUploadCommand(data.getMessageType()))
{
    state = IDLE;
    retransmissionCounter = 0;
}

void UploadSignalPayload::start(void)
{
    monitor->trace("Upload signal data payload procedure requested");
    state = INITIAL_COMMAND;
    sendSignal(command, SignalData::START);
}

bool UploadSignalPayload::isActionDone(void) const
{
    return IDLE == state;
}

ICommAction::Type UploadSignalPayload::getType(void) const
{
    return UPLOAD_SIGNAL_PAYLOAD;
}

std::string UploadSignalPayload::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    case UPLOAD: return "UPLOAD";
    default:
        __RL_EXCEPTION__("UploadSignalPayload::getStateName::Unexpected state");
    }
}

void UploadSignalPayload::handleReception(const IMessage& message)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        handleIdleReception(message);
        break;

    default:
        except("Message received in unexpected state", message);
    }
}

void UploadSignalPayload::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Upload signal data payload procedure started");
            state = UPLOAD;
            listener->send(data);
            startSignalTimeout(data.getSignalDataCommand(), DEFAULT_SIGNAL_TIMEOUT + 500);
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    case UPLOAD:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Upload successfull");
            monitor->notifyUavEvent(new UavEventSent(data));
            state = IDLE;
            listener->startAction(new AppAction(listener));
            break;

        case SignalData::DATA_INVALID:
        case SignalData::TIMEOUT:
            retransmissionCounter++;
            monitor->trace("Receiver reports file over signal payload, errors: " +
                           std::to_string(retransmissionCounter) + " / " + std::to_string(MAX_SIGNAL_PAYLOAD_RECEPTION_ERRORS));
            if (retransmissionCounter < MAX_SIGNAL_PAYLOAD_RECEPTION_ERRORS)
            {
                monitor->trace("Upload timeout, retring");
                listener->send(data);
                startSignalTimeout(data.getSignalDataCommand(), DEFAULT_SIGNAL_TIMEOUT + 500);
            }
            else
            {
                except("Retransmission counter exceeded when uploading signal paylod data", parameter);
            }
            break;

        default:
            except("Unexpected signal parameter received", parameter);
        }
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}

SignalData::Command UploadSignalPayload::getUploadCommand(const IMessage::MessageType type) const
{
    switch (type)
    {
    case IMessage::CONTROL_SETTINGS: return SignalData::UPLOAD_SETTINGS;
    case IMessage::ROUTE_CONTAINER: return SignalData::UPLOAD_ROUTE;
    case IMessage::WIFI_CONFIGURATION: return SignalData::WIFI_CONFIGURATION;
    default:
        __RL_EXCEPTION__("DownloadSignalPaylod::getDownloadCommand::Unexpected type");
    }
}
