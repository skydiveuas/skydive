#include "SignalData.hpp"

#include "CalibrationSettings.hpp"
#include "ControlSettings.hpp"
#include "RouteContainer.hpp"

#include <string.h>

SignalData::SignalData(void)
{
}

SignalData::SignalData(const unsigned char* src)
{
    command = parseCommand(src);
    parameter = parseParameter(src + IMessage::SIGNAL_COMMAND_SIZE);
}

SignalData::SignalData(const Command& _command, const Parameter& _parameter) :
    command(_command), parameter(_parameter)
{
}

SignalData::SignalData(const Command& _command, const int _parameterValue) :
    command(_command), parameter(_parameterValue)
{
}

SignalData::Command SignalData::getCommand(void) const
{
    return static_cast<Command>(command);
}

SignalData::Parameter SignalData::getParameter(void) const
{
    return static_cast<Parameter>(parameter);
}

int SignalData::getParameterValue() const
{
    return parameter;
}

bool SignalData::operator ==(const SignalData& right) const
{
    return getCommand() == right.getCommand()
            && getParameter() == right.getParameter();
}

unsigned SignalData::getPayloadSize(void) const
{
    return IMessage::getPayloadSizeByType(getPreambleType());
}

IMessage::PreambleType SignalData::getPreambleType(void) const
{
    return IMessage::SIGNAL;
}

void SignalData::serialize(unsigned char* dst) const
{
    memcpy(dst, &command, IMessage::SIGNAL_COMMAND_SIZE);
    memcpy(dst + IMessage::SIGNAL_COMMAND_SIZE, &parameter, IMessage::SIGNAL_COMMAND_SIZE);
}

IMessage::MessageType SignalData::getMessageType(void) const
{
    return SIGNAL_DATA;
}

SignalData::Command SignalData::parseCommand(const unsigned char* src)
{
    int commandValue;
    memcpy(&commandValue, src, IMessage::SIGNAL_COMMAND_SIZE);
    return static_cast<Command>(commandValue);
}

SignalData::Parameter SignalData::parseParameter(const unsigned char* src)
{
    int parameterValue;
    memcpy(&parameterValue, src, IMessage::SIGNAL_COMMAND_SIZE);
    return static_cast<Parameter>(parameterValue);
}

unsigned short SignalData::parseAllPacketsNumber(const unsigned char* src)
{
    unsigned short result = 0;
    memcpy(&result, src + IMessage::SIGNAL_COMMAND_SIZE, 2);
    return result;
}

unsigned short SignalData::parseActualPacketNumber(const unsigned char* src)
{
    unsigned short result = 0;
    memcpy(&result, src + IMessage::SIGNAL_COMMAND_SIZE + 2, 2);
    return result;
}

bool SignalData::hasPayload(const SignalData::Command command)
{
    switch (command)
    {
    case CALIBRATION_SETTINGS_DATA:
    case CONTROL_SETTINGS_DATA:
    case ROUTE_CONTAINER_DATA:
    case WIFI_CONFIGURATION_DATA:
        return true;

    default:
        return false;
    }
}

#ifdef __SKYDIVE_USE_STL__

std::string SignalData::toString() const
{
    return toString(*this);
}

std::string SignalData::toString(const SignalData& command)
{
    return "SIGNAL_DATA(" + toString(command.getCommand())
            + ":"
            + toString(command.getParameter()) + ")";
}

std::string SignalData::toString(const SignalData::Command& command)
{
    switch (command)
    {
    case SignalData::DUMMY:
        return std::string("DUMMY");
    case SignalData::START_CMD:
        return std::string("START_COMMAND");
    case SignalData::APP_LOOP:
        return std::string("APPLICATION_LOOP");
    case SignalData::FLIGHT_LOOP:
        return std::string("FLIGHT_LOOP");
    case SignalData::CALIBRATE_ACCEL:
        return std::string("CALIBRATE_ACCELEROMETER");
    case SignalData::CALIBRATE_MAGNET:
        return std::string("CALIBRATE_MAGNETOMETER");
    case SignalData::CALIBRATE_ESC:
        return std::string("CALIBRATE_ESC");
    case SignalData::UPLOAD_SETTINGS:
        return std::string("UPLOAD_SETTINGS");
    case SignalData::DOWNLOAD_SETTINGS:
        return std::string("DOWNLOAD_SETTINGS");
    case SignalData::CALIBRATE_RADIO:
        return std::string("CALIBRATE_RADIO");
    case SignalData::CHECK_RADIO:
        return std::string("CHECK_RADIO");
    case SignalData::SOFTWARE_UPGRADE:
        return std::string("SOFTWARE_UPGRADE");
    case SignalData::SYSTEM_RESET:
        return std::string("SYSTEM_RESET");
    case SignalData::UPLOAD_ROUTE:
        return std::string("UPLOAD_ROUTE");
    case SignalData::DOWNLOAD_ROUTE:
        return std::string("DOWNLOAD_ROUTE");
    case SignalData::CONFIGURE_WIFI:
        return std::string("CONFIGURE_WIFI");
    case SignalData::SENSORS_LOGGER:
        return std::string("SENSORS_LOGGER");
    case SignalData::CALIBRATION_SETTINGS:
        return std::string("CALIBRATION_SETTINGS");
    case SignalData::CONTROL_SETTINGS:
        return std::string("CONTROL_SETTINGS");
    case SignalData::ROUTE_CONTAINER:
        return std::string("ROUTE_CONTAINER");
    case SignalData::CALIBRATION_SETTINGS_DATA:
        return std::string("CALIBRATION_SETTINGS_DATA");
    case SignalData::CONTROL_SETTINGS_DATA:
        return std::string("CONTROL SETTINGS_DATA");
    case SignalData::ROUTE_CONTAINER_DATA:
        return std::string("ROUTE_CONTAINER_DATA");
    case SignalData::WIFI_CONFIGURATION:
        return std::string("WIFI_CONFIGURATION");
    case SignalData::WIFI_CONFIGURATION_DATA:
        return std::string("WIFI_CONFIGURATION_DATA");
    case SignalData::WHO_AM_I_VALUE:
        return std::string("WHO_AM_I_VALUE");
    default:
        return std::string("Bad command type");
    }
}

std::string SignalData::toString(const SignalData::Parameter& parameter)
{
    switch (parameter)
    {
    case SignalData::DUMMY_PARAMETER:
        return std::string("DUMMY PARAMETER");
    case SignalData::START:
        return std::string("START");
    case SignalData::ACK:
        return std::string("ACK");
    case SignalData::DATA_ACK:
        return std::string("DATA_ACK");
    case SignalData::BREAK:
        return std::string("BREAK");
    case SignalData::BREAK_ACK:
        return std::string("BREAK_ACK");
    case SignalData::BREAK_FAIL:
        return std::string("BREAK_FAIL");
    case SignalData::DONE:
        return std::string("DONE");
    case SignalData::READY:
        return std::string("READY");
    case SignalData::FAIL:
        return std::string("FAIL");
    case SignalData::SKIP:
        return std::string("SKIP");
    case SignalData::NON_STATIC:
        return std::string("NON_STATIC");
    case SignalData::NOT_ALLOWED:
        return std::string("NOT_ALLOWED");
    case SignalData::DATA_INVALID:
        return std::string("DATA_INVALID");
    case SignalData::TIMEOUT:
        return std::string("TIMEOUT");
    case SignalData::VIA_ROUTE_ALLOWED:
        return std::string("VIA_ROUTE_ALLOWED");
    case SignalData::VIA_ROUTE_NOT_ALLOWED:
        return std::string("VIA_ROUTE_NOT_ALLOWED");
    default:
        return std::string("Bad parameter type");
    }
}

#endif //__SKYDIVE_USE_STL__
