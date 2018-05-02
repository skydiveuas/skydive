// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#include "endpoint/device/DeviceEvent.hpp"

DeviceEvent::DeviceEvent(const Type& _type):
    type(_type)
{
}

DeviceEvent::~DeviceEvent(void)
{
}

DeviceEvent::Type DeviceEvent::getType(void) const
{
    return type;
}

std::string DeviceEvent::toString(void) const
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
        __SKY_EXCEPTION__("UavEvent::toString:Unexpected event type");
    }
}

DeviceEventMessage::DeviceEventMessage(const MessageType& _messageType, const std::string& _message):
    DeviceEvent(MESSAGE),
    messageType(_messageType),
    message(_message)
{
}

bool DeviceEventMessage::hasMessage(void) const
{
    return true;
}

DeviceEventMessage::MessageType DeviceEventMessage::getMessageType(void) const
{
    return messageType;
}

std::string DeviceEventMessage::getMessage(void) const
{
    return message;
}

std::string DeviceEventMessage::toString(void) const
{
    return DeviceEvent::toString() + " with:\n\"" + message + "\"";
}

DeviceEventReceived::DeviceEventReceived(const IMessage& _received) :
    DeviceEvent(DATA_RECEIVED),
    received(_received)
{
}

DeviceEventReceived::~DeviceEventReceived(void)
{
    delete &received;
}

const IMessage& DeviceEventReceived::getReceived(void) const
{
    return received;
}

std::string DeviceEventReceived::toString(void) const
{
    return DeviceEvent::toString() + " with: " + received.getMessageName();
}

DeviceEventSent::DeviceEventSent(const IMessage& _sent) :
    DeviceEvent(DATA_SENT),
    sent(_sent)
{
}

DeviceEventSent::~DeviceEventSent(void)
{
    delete &sent;
}

const IMessage& DeviceEventSent::getSent(void) const
{
    return sent;
}

std::string DeviceEventSent::toString(void) const
{
    return DeviceEvent::toString() + " with: " + sent.getMessageName();
}

DeviceEventConnectionStatus::DeviceEventConnectionStatus(const unsigned _ping,
                                                         const unsigned _received,
                                                         const unsigned _fails) :
    DeviceEvent(CONNECTION_STATUS),
    ping(_ping),
    received(_received),
    fails(_fails)
{
}

unsigned DeviceEventConnectionStatus::getPing(void) const
{
    return ping;
}

unsigned DeviceEventConnectionStatus::getReceived(void) const
{
    return received;
}

unsigned DeviceEventConnectionStatus::getFails(void) const
{
    return fails;
}

std::string DeviceEventConnectionStatus::toString(void) const
{
    return DeviceEvent::toString() + ": " + std::to_string(ping) + " ms";
}

DeviceEventUpgradeStarted::DeviceEventUpgradeStarted(const CalibrationSettings::BoardType _type) :
    DeviceEvent(UPGRADE_STARTED),
    boardType(_type)
{
}

CalibrationSettings::BoardType DeviceEventUpgradeStarted::getBoardType(void) const
{
    return boardType;
}

std::string DeviceEventUpgradeStarted::toString(void) const
{
    return DeviceEvent::toString() + ": " + CalibrationSettings::getBoardTypeString(boardType);
}

DeviceEventWhoAmI::DeviceEventWhoAmI(const CalibrationSettings::BoardType _type) :
    DeviceEvent(WHO_AM_I),
    boardType(_type)
{
}

CalibrationSettings::BoardType DeviceEventWhoAmI::getBoardType(void) const
{
    return boardType;
}

std::string DeviceEventWhoAmI::toString(void) const
{
    return DeviceEvent::toString() + ": " + CalibrationSettings::getBoardTypeString(boardType);
}
