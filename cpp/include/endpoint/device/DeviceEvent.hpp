// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef DEVICEEVENT_HPP
#define DEVICEEVENT_HPP

#include "communication/IMessage.hpp"
#include "communication/ISignalPayloadMessage.hpp"

#include "communication/CalibrationSettings.hpp"

#include <string>

/**
 * =============================================================================================
 * UavEvent
 * =============================================================================================
 */
class DeviceEvent
{
public:
    // events that can happen on UAV side
    enum Type
    {
        MESSAGE,
        DATA_RECEIVED,
        DATA_SENT,
        CONNECTION_STATUS,
        WHO_AM_I,
        CALIBRATION_NON_STATIC,
        APPLICATION_LOOP_STARTED,
        APPLICATION_LOOP_ENDED,
        APPLICATION_LOOP_TERMINATED,
        FLIGHT_LOOP_NOT_ALLOWED,
        VIA_ROUTE_NOT_ALLOWED,
        FLIGHT_LOOP_STARTED,
        FLIGHT_LOOP_ENDED,
        FLIGHT_LOOP_TERMINATED,
        CALIBRATE_MAGNET_STARTED,
        CALIBRATE_RADIO_STARTED,
        CALIBRATE_RADIO_NOT_ALLOWED,
        CALIBRATE_RADIO_ENDED,
        CALIBRATE_RADIO_ACK,
        CALIBRATE_RADIO_FAIL,
        CHECK_RADIO_STARTED,
        CHECK_RADIO_NOT_ALLOWED,
        CHECK_RADIO_ENDED,
        UPGRADE_STARTED,
        SENSORS_LOGGER_STARTED,
        SENSORS_LOGGER_ENDED,
        WIFI_CONFIG_ENDED,
        CONNECTION_LOST,
        CONNECTION_RECOVERED,
        CONTROLS_DOWNLOAD_FAIL,
        ROUTE_DOWNLOAD_FAIL,
        CALIBRATE_ESC_STARTED,
        CALIBRATE_ESC_NOT_ALLOWED,
        CALIBRATE_ESC_DONE,
        CALIBRATE_ESC_ENDED
    };

    const Type type;

    DeviceEvent(const Type& _type);

    virtual ~DeviceEvent(void);

    Type getType(void) const;

    virtual std::string toString(void) const;
};

class DeviceEventMessage : public DeviceEvent
{
public:
    enum MessageType
    {
        INFO,
        WARNING,
        ERROR
    };

private:
    const MessageType messageType;
    const std::string message;

public:
    DeviceEventMessage(const MessageType& _messageType, const std::string& _message);

    bool hasMessage(void) const;
    MessageType getMessageType(void) const;
    std::string getMessage(void) const;

    std::string toString(void) const;
};

class DeviceEventReceived : public DeviceEvent
{
private:
    const IMessage& received;

public:
    DeviceEventReceived(const IMessage& _received);

    ~DeviceEventReceived(void);

    const IMessage& getReceived(void) const;

    std::string toString(void) const;
};

class DeviceEventSent : public DeviceEvent
{
private:
    const IMessage& sent;

public:
    DeviceEventSent(const IMessage& _sent);

    ~DeviceEventSent(void);

    const IMessage& getSent(void) const;

    std::string toString(void) const;
};

class DeviceEventConnectionStatus : public DeviceEvent
{
private:
    const unsigned ping;
    const unsigned received;
    const unsigned fails;

public:
    DeviceEventConnectionStatus(const unsigned _ping,
                                const unsigned _received,
                                const unsigned _fails);

    unsigned getPing(void) const;
    unsigned getReceived(void) const;
    unsigned getFails(void) const;

    std::string toString(void) const;
};

class DeviceEventUpgradeStarted : public DeviceEvent
{
private:
    const CalibrationSettings::BoardType boardType;

public:
    DeviceEventUpgradeStarted(const CalibrationSettings::BoardType _type);

    CalibrationSettings::BoardType getBoardType(void) const;

    std::string toString(void) const;
};

class DeviceEventWhoAmI : public DeviceEvent
{
private:
    const CalibrationSettings::BoardType boardType;

public:
    DeviceEventWhoAmI(const CalibrationSettings::BoardType _type);

    CalibrationSettings::BoardType getBoardType(void) const;

    std::string toString(void) const;
};

#endif // DEVICEEVENT_HPP
