#include "FlightInitializationAction.hpp"

#include "AppAction.hpp"
#include "FlightAction.hpp"

#include "Exception.hpp"

FlightInitializationAction::FlightInitializationAction(Listener* const _listener):
    ICommAction(_listener)
{
    state = IDLE;
}

void FlightInitializationAction::start(void)
{
    monitor->trace("Flight initialization procedure started");
    state = INITIAL_COMMAND;
    sendSignal(SignalData::FLIGHT_LOOP, SignalData::START);
}

bool FlightInitializationAction::isActionDone(void) const
{
    return IDLE == state;
}

ICommAction::Type FlightInitializationAction::getType(void) const
{
    return FLIGHT_INITIALIZATION;
}

std::string FlightInitializationAction::getStateName(void) const
{
    switch (state)
    {
    case IDLE: return "IDLE";
    case INITIAL_COMMAND: return "INITIAL_COMMAND";
    case CONTROLS_RECEPTION: return "CONTROLS_RECEPTION";
    case ROUTE_COMMAND: return "ROUTE_COMMAND";
    case ROUTE_RECEPTION: return "ROUTE_RECEPTION";
    default:
        __RL_EXCEPTION__("FlightAction::getStateName::Unexpected state");
    }
}

void FlightInitializationAction::handleReception(const IMessage& message)
{
    switch (state)
    {
    case IDLE:
    case INITIAL_COMMAND:
        handleIdleReception(message);
        break;

    case CONTROLS_RECEPTION:
        if (handleSignalPayloadReception(message))
        {
            monitor->notifyUavEvent(new UavEventReceived(message));
            startSignalTimeout(SignalData::FLIGHT_LOOP);
            state = ROUTE_COMMAND;
        }
        break;

    case ROUTE_RECEPTION:
        if (handleSignalPayloadReception(message))
        {
            monitor->notifyUavEvent(new UavEventReceived(message));
            flightReady();
        }
        break;

    default:
        except("Message received in unexpected state", message);
    }
}

void FlightInitializationAction::handleSignalReception(const Parameter parameter)
{
    switch (state)
    {
    case INITIAL_COMMAND:
        switch (parameter)
        {
        case SignalData::ACK:
            monitor->trace("Flight initial command received");
            initializeSignalPayloadReception(SignalData::CONTROL_SETTINGS);
            state = CONTROLS_RECEPTION;
            break;

        case SignalData::NOT_ALLOWED:
            monitor->notifyUavEvent(new UavEvent(UavEvent::FLIGHT_LOOP_NOT_ALLOWED));
            state = IDLE;
            listener->startAction(new AppAction(listener));
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    case ROUTE_COMMAND:
        switch (parameter)
        {
        case SignalData::VIA_ROUTE_ALLOWED:
            monitor->trace("Via route allowed, receiving route");
            initializeSignalPayloadReception(SignalData::ROUTE_CONTAINER);
            state = ROUTE_RECEPTION;
            break;

        case SignalData::VIA_ROUTE_NOT_ALLOWED:
            monitor->notifyUavEvent(new UavEvent(UavEvent::VIA_ROUTE_NOT_ALLOWED));
            flightReady();
            break;

        default:
            except("Unexpected signal parameter message received", parameter);
        }
        break;

    default:
        except("Signal parameter in unexpected state", parameter);
    }
}

void FlightInitializationAction::flightReady(void)
{
    monitor->trace("Flight initialization procedure done, switching to FLIGHT");
    state = IDLE;
    listener->startAction(new FlightAction(listener, monitor->getControlDataSendingFreq()));
}
