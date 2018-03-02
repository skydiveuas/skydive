// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#include "UavEvent.hpp"

UavEvent::UavEvent(const Type& _type):
    type(_type)
{
}

UavEvent::~UavEvent(void)
{
}

UavEvent::Type UavEvent::getType(void) const
{
    return type;
}

std::string UavEvent::toString(void) const
{
    switch (type)
    {
    case MESSAGE: return "MESSAGE";
    case DATA_RECEIVED: return "DATA_RECEIVED";
    case DATA_SENT: return "DATA_SENT";
    case CONNECTION_STATUS: return "CONNECTION_STATUS";
    case WHO_AM_I: return "WHO_AM_I";
    case CALIBRATION_NON_STATIC: return "CALIBRATION_NON_STATIC";
    case APPLICATION_LOOP_STARTED: return "APPLICATION_LOOP_STARTED";
    case APPLICATION_LOOP_ENDED: return "APPLICATION_LOOP_ENDED";
    case APPLICATION_LOOP_TERMINATED: return "APPLICATION_LOOP_TERMINATED";
    case FLIGHT_LOOP_NOT_ALLOWED: return "FLIGHT_LOOP_NOT_ALLOWED";
    case VIA_ROUTE_NOT_ALLOWED: return "VIA_ROUTE_NOT_ALLOWED";
    case FLIGHT_LOOP_STARTED: return "FLIGHT_LOOP_STARTED";
    case FLIGHT_LOOP_ENDED: return "FLIGHT_LOOP_ENDED";
    case FLIGHT_LOOP_TERMINATED: return "FLIGHT_LOOP_TERMINATED";
    case CALIBRATE_MAGNET_STARTED: return "CALIBRATE_MAGNET_STARTED";
    case CALIBRATE_RADIO_STARTED: return "CALIBRATE_RADIO_STARTED";
    case CALIBRATE_RADIO_NOT_ALLOWED: return "CALIBRATE_RADIO_NOT_ALLOWED";
    case CALIBRATE_RADIO_ENDED: return "CALIBRATE_RADIO_ENDED";
    case CALIBRATE_RADIO_ACK: return "CALIBRATE_RADIO_ACK";
    case CALIBRATE_RADIO_FAIL: return "CALIBRATE_RADIO_FAIL";
    case CHECK_RADIO_STARTED: return "CHECK_RADIO_STARTED";
    case CHECK_RADIO_NOT_ALLOWED: return "CHECK_RADIO_NOT_ALLOWED";
    case CHECK_RADIO_ENDED: return "CHECK_RADIO_ENDED";
    case UPGRADE_STARTED: return "UPGRADE_STARTED";
    case SENSORS_LOGGER_STARTED: return "SENSORS_LOGGER_STARTED";
    case SENSORS_LOGGER_ENDED: return "SENSORS_LOGGER_ENDED";
    case WIFI_CONFIG_ENDED: return "WIFI_CONFIG_ENDED";
    case CONNECTION_LOST: return "CONNECTION_LOST";
    case CONNECTION_RECOVERED: return "CONNECTION_RECOVERED";
    case CONTROLS_DOWNLOAD_FAIL: return "CONTROLS_DOWNLOAD_FAIL";
    case ROUTE_DOWNLOAD_FAIL: return "ROUTE_DOWNLOAD_FAIL";
    case CALIBRATE_ESC_STARTED: return "CALIBRATE_ESC_STARTED";
    case CALIBRATE_ESC_NOT_ALLOWED: return "CALIBRATE_ESC_NOT_ALLOWED";
    case CALIBRATE_ESC_DONE: return "CALIBRATE_ESC_DONE";
    case CALIBRATE_ESC_ENDED: return "CALIBRATE_ESC_ENDED";
    default:
        __RL_EXCEPTION__("UavEvent::toString:Unexpected event type");
    }
}

UavEventMessage::UavEventMessage(const MessageType& _messageType, const std::string& _message):
    UavEvent(MESSAGE),
    messageType(_messageType),
    message(_message)
{
}

bool UavEventMessage::hasMessage(void) const
{
    return true;
}

UavEventMessage::MessageType UavEventMessage::getMessageType(void) const
{
    return messageType;
}

std::string UavEventMessage::getMessage(void) const
{
    return message;
}

std::string UavEventMessage::toString(void) const
{
    return UavEvent::toString() + " with:\n\"" + message + "\"";
}

UavEventReceived::UavEventReceived(const IMessage& _received) :
    UavEvent(DATA_RECEIVED),
    received(_received)
{
}

UavEventReceived::~UavEventReceived(void)
{
    delete &received;
}

const IMessage& UavEventReceived::getReceived(void) const
{
    return received;
}

std::string UavEventReceived::toString(void) const
{
    return UavEvent::toString() + " with: " + received.getMessageName();
}

UavEventSent::UavEventSent(const IMessage& _sent) :
    UavEvent(DATA_SENT),
    sent(_sent)
{
}

UavEventSent::~UavEventSent(void)
{
    delete &sent;
}

const IMessage& UavEventSent::getSent(void) const
{
    return sent;
}

std::string UavEventSent::toString(void) const
{
    return UavEvent::toString() + " with: " + sent.getMessageName();
}

UavEventConnectionStatus::UavEventConnectionStatus(const unsigned _ping,
                                                   const unsigned _received,
                                                   const unsigned _fails) :
    UavEvent(CONNECTION_STATUS),
    ping(_ping),
    received(_received),
    fails(_fails)
{
}

unsigned UavEventConnectionStatus::getPing(void) const
{
    return ping;
}

unsigned UavEventConnectionStatus::getReceived(void) const
{
    return received;
}

unsigned UavEventConnectionStatus::getFails(void) const
{
    return fails;
}

std::string UavEventConnectionStatus::toString(void) const
{
    return UavEvent::toString() + ": " + std::to_string(ping) + " ms";
}

UavEventWhoAmI::UavEventWhoAmI(const CalibrationSettings::BoardType _type) :
    UavEvent(WHO_AM_I),
    boardType(_type)
{
}

CalibrationSettings::BoardType UavEventWhoAmI::getBoardType(void) const
{
    return boardType;
}

std::string UavEventWhoAmI::toString(void) const
{
    return UavEvent::toString() + ": " + CalibrationSettings::getBoardTypeString(boardType);
}
