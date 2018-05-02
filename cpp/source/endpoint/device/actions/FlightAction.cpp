#include "endpoint/device/actions/FlightAction.hpp"

#include "endpoint/device/actions/AppAction.hpp"

#include "endpoint/device/PilotEvent.hpp"

#include "common/SkyException.hpp"

#include <functional>

FlightAction::FlightAction(Listener* const _listener, const double _controlFreq):
    ISkyDeviceAction(_listener),
    controlFreq(_controlFreq)
{
    state = IDLE;

    controlTimer = monitor->createTimer(std::bind(&FlightAction::controlTaskHandler, this));
}

FlightAction::~FlightAction(void)
{
    delete controlTimer;
}

void FlightAction::start(void)
{
    monitor->trace("Flight final start command");
    state = FLING;
    monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::FLIGHT_LOOP_STARTED));
    listener->send(SignalData(SignalData::FLIGHT_LOOP, SignalData::READY));
    listener->enablePingTask(true);
    controlTimer->start(controlFreq);
}

bool FlightAction::isActionDone(void) const
{
    return IDLE == state;
}

ISkyDeviceAction::Type FlightAction::getType(void) const
{
    return FLIGHT;
}

std::string FlightAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case FLING: return "FLING";
    case BREAKING: return "BREAKING";
    default:
        __SKY_EXCEPTION__("FlightAction::getStateName::Unexpected state");
    }
}

void FlightAction::handleReception(const IMessage& message)
{
    switch (state)
    {
    case IDLE:
        handleIdleReception(message);
        break;

    case FLING:
    case BREAKING:
        handleRunningReception(message);
        break;

    default:
        except("Unexpected state");
    }
}

void FlightAction::handleUserEvent(const PilotEvent& event)
{
    switch (state)
    {
    case FLING:
        switch (event.getType())
        {
        case PilotEvent::BREAK_FLIGHT_LOOP:
            listener->enablePingTask(false);
            listener->enableConnectionTimeoutTask(false);
            state = BREAKING;
            break;

        case PilotEvent::AUTOPILOT:
            sendAutopilotTarget(reinterpret_cast<const PilotEventAutopilot&>(event));
            break;

        default:
            except("Unexpected user event received");
        }
        break;

    default:
        except("User event received in unexpected state");
    }
}

void FlightAction::handleRunningReception(const IMessage& message)
{
    switch (message.getMessageType())
    {
    case IMessage::DEBUG_DATA:
        monitor->notifyDeviceEvent(new DeviceEventReceived(message));
        break;

    case IMessage::AUTOPILOT_DATA:
        monitor->notifyDeviceEvent(new DeviceEventReceived(message));
        handleAutopilotReception(reinterpret_cast<const AutopilotData&>(message));
        break;

    case IMessage::SIGNAL_DATA:
    {
        const SignalData signal = reinterpret_cast<const SignalData&>(message);
        switch (signal.getCommand())
        {
        case SignalData::PING_VALUE:
            listener->onPongReception(signal);
            break;

        case SignalData::FLIGHT_LOOP:
            if (SignalData::BREAK_ACK == signal.getParameter() && BREAKING == state)
            {
                flightEnded(false);
            }
            else if (SignalData::BREAK == signal.getParameter())
            {
                flightEnded(true);
            }
            else
            {
                except("Unexpected message received", message);
            }
            break;

        default:
            except("Unexpected message received", message);
        }
        break;
    }

    default:
        except("Unexpected message received", message);
    }
}

void FlightAction::handleAutopilotReception(const AutopilotData& message)
{
    switch (message.getType())
    {
    case AutopilotData::BASE:
        sendBaseConfirmation(message);
        break;

    case AutopilotData::TARGET_ACK:
        monitor->trace("Target confirmed by board, stopping timeout timer");
        break;

    case AutopilotData::TARGET_NOT_ALLOWED_SETTINGS:
    case AutopilotData::TARGET_NOT_ALLOWED_STATE:
        monitor->trace("Target rejected by board, stopping timeout timer");
        break;

    default:
        except("Unexpected autopilot message received", message);
    }
}

void FlightAction::controlTaskHandler(void)
{
    std::unique_ptr<ControlData> data(new ControlData(monitor->getControlDataToSend()));
    if (BREAKING == state)
    {
        monitor->trace("FlightAction::controlTaskHandler:setting break command");
        data->setControllerCommand(ControlData::STOP);
    }
    else if (data->getControllerCommand() == ControlData::STOP)
    {
        monitor->trace("FlightAction::controlTaskHandler:setting break state");
        state = BREAKING;
    }
    listener->send(*data);
    monitor->notifyDeviceEvent(new DeviceEventSent(*data.release()));
}

void FlightAction::flightEnded(const bool byBoard)
{
    if (byBoard)
    {
        monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::FLIGHT_LOOP_TERMINATED));
    }
    else
    {
        monitor->notifyDeviceEvent(new DeviceEvent(DeviceEvent::FLIGHT_LOOP_ENDED));
    }
    listener->enablePingTask(false);
    controlTimer->stop();
    state = IDLE;
    listener->startAction(new AppAction(listener));
}

void FlightAction::sendAutopilotTarget(const PilotEventAutopilot& event)
{
    monitor->trace("Sending autopilot data event");
    std::unique_ptr<AutopilotData> data(new AutopilotData());
    data->setType(AutopilotData::TARGET);
    data->setTargetPosition(event.getPosition());
    data->flags().setFlagState(AutopilotData::ALTITUDE_DEFINED, false);
    listener->send(*data);
    monitor->notifyDeviceEvent(new DeviceEventSent(*data.release()));
}

void FlightAction::sendBaseConfirmation(const AutopilotData& base)
{
    monitor->trace("Base location received, responding with ACK");
    std::unique_ptr<AutopilotData> data(new AutopilotData(base));
    data->setType(AutopilotData::BASE_ACK);
    listener->send(*data);
    monitor->notifyDeviceEvent(new DeviceEventSent(*data.release()));
}
