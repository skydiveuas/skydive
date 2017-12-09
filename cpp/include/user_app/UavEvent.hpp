#ifndef __UAV_EVENT__
#define __UAV_EVENT__

#include "IMessage.hpp"
#include "ISignalPayloadMessage.hpp"

#include "CalibrationSettings.hpp"

#include "Exception.hpp"

#include <string>

/**
 * =============================================================================================
 * UavEvent
 * =============================================================================================
 */
class UavEvent
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

    UavEvent(const Type& _type);

    virtual ~UavEvent(void);

    Type getType(void) const;

    virtual std::string toString(void) const;
};

class UavEventMessage : public UavEvent
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
    UavEventMessage(const MessageType& _messageType, const std::string& _message);

    bool hasMessage(void) const;
    MessageType getMessageType(void) const;
    std::string getMessage(void) const;

    std::string toString(void) const;
};

class UavEventReceived : public UavEvent
{
private:
    const IMessage& received;

public:
    UavEventReceived(const IMessage& _received);

    ~UavEventReceived(void);

    const IMessage& getReceived(void) const;

    std::string toString(void) const;
};

class UavEventSent : public UavEvent
{
private:
    const IMessage& sent;

public:
    UavEventSent(const IMessage& _sent);

    ~UavEventSent(void);

    const IMessage& getSent(void) const;

    std::string toString(void) const;
};

class UavEventConnectionStatus : public UavEvent
{
private:
    const unsigned ping;
    const unsigned received;
    const unsigned fails;

public:
    UavEventConnectionStatus(const unsigned _ping,
                             const unsigned _received,
                             const unsigned _fails);

    unsigned getPing(void) const;
    unsigned getReceived(void) const;
    unsigned getFails(void) const;

    std::string toString(void) const;
};

class UavEventWhoAmI : public UavEvent
{
private:
    const CalibrationSettings::BoardType boardType;

public:
    UavEventWhoAmI(const CalibrationSettings::BoardType _type);

    CalibrationSettings::BoardType getBoardType(void) const;

    std::string toString(void) const;
};

#endif // __UAV_EVENT__
