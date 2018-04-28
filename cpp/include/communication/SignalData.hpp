// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __SIGNAL_DATA__
#define __SIGNAL_DATA__

#include "IMessage.hpp"

#ifdef __SKYDIVE_USE_STL__

#include <string>

#endif //__SKYDIVE_USE_STL__

/**
 * =============================================================================================
 * SignalData
 * =============================================================================================
 */
class SignalData : public IMessage
{
public:
    enum Command
    {
        DUMMY,

        START_CMD = 100007,

        APP_LOOP,
        FLIGHT_LOOP,
        CALIBRATE_ACCEL,
        CALIBRATE_MAGNET,
        CALIBRATE_ESC,
        UPLOAD_SETTINGS,
        DOWNLOAD_SETTINGS,
        CALIBRATE_RADIO,
        CHECK_RADIO,
        SOFTWARE_UPGRADE,
        SYSTEM_RESET,
        UPLOAD_ROUTE,
        DOWNLOAD_ROUTE,
        CONFIGURE_WIFI,
        SENSORS_LOGGER,

        PING_VALUE, // random ping key

        CALIBRATION_SETTINGS,
        CALIBRATION_SETTINGS_DATA,
        CONTROL_SETTINGS,
        CONTROL_SETTINGS_DATA,
        ROUTE_CONTAINER,
        ROUTE_CONTAINER_DATA,
        WIFI_CONFIGURATION,
        WIFI_CONFIGURATION_DATA,

        // TODO sort these values in new lib release
        WHO_AM_I_VALUE, // board type (CalibrationSettings::BoardType)
        PROTOCOL_VERSION_VALUE,
        PROTOCOL_VERSION
    };

    enum Parameter
    {
        DUMMY_PARAMETER,

        START = 1000011,
        ACK,
        DATA_ACK,
        ENTER_DFU,

        BREAK,
        BREAK_ACK,
        BREAK_FAIL,

        DONE,
        READY,
        FAIL,
        SKIP,

        NON_STATIC,
        NOT_ALLOWED,

        DATA_INVALID,
        TIMEOUT,

        VIA_ROUTE_ALLOWED,
        VIA_ROUTE_NOT_ALLOWED
    };

    SignalData(void);
    SignalData(const unsigned char* src);
    SignalData(const Command& command, const Parameter& parameter);
    SignalData(const Command& command, const int parameterValue);

    Command getCommand(void) const;
    Parameter getParameter(void) const;

    int getParameterValue() const;

    bool operator ==(const SignalData& right) const;

    unsigned getPayloadSize(void) const;

    PreambleType getPreambleType(void) const;

    void serialize(unsigned char* dst) const;

    MessageType getMessageType(void) const;

    static Command parseCommand(const unsigned char* src);
    static Parameter parseParameter(const unsigned char* src);

    static unsigned short parseAllPacketsNumber(const unsigned char* src);
    static unsigned short parseActualPacketNumber(const unsigned char* src);

    static bool hasPayload(const Command command);

#ifdef __SKYDIVE_USE_STL__

    std::string toString(void) const override;

    static std::string toString(const SignalData& command);
    static std::string toString(const SignalData::Command& command);
    static std::string toString(const SignalData::Parameter& parameter);

#endif //__SKYDIVE_USE_STL__

private:
    int command;
    int parameter;
};


#endif // __SIGNAL_DATA__
