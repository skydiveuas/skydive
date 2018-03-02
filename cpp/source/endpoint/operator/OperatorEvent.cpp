// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#include "UserUavEvent.hpp"

UserUavEvent::UserUavEvent(const Type& _type) :
    type(_type)
{
}

UserUavEvent::~UserUavEvent(void)
{
}

UserUavEvent::Type UserUavEvent::getType(void) const
{
    return type;
}

std::string UserUavEvent::toString(void) const
{
    switch (type)
    {
    case CONNECT: return "CONNECT";
    case ACTION: return "ACTION";
    case UPLOAD: return "UPLOAD";
    case DOWNLOAD: return "DOWNLOAD";
    case AUTOPILOT: return "AUTOPILOT";
    case MAGNET_CALIB_DONE: return "MAGNET_CALIB_DONE";
    case MAGNET_CALIB_ABORT: return "MAGNET_CALIB_ABORT";
    case BREAK_APPLICATION_LOOP: return "BREAK_APPLICATION_LOOP";
    case BREAK_FLIGHT_LOOP: return "BREAK_FLIGHT_LOOP";
    case BREAK_RADIO_CHECK: return "BREAK_RADIO_CHECK";
    case BREAK_SENSORS_LOGGER: return "BREAK_SENSORS_LOGGER";
    case ESC_CALIB_DONE: return "ESC_CALIB_DONE";
    case ESC_CALIB_ABORT: return "ESC_CALIB_ABORT";
    case RADIO_CHECK_DONE: return "RADIO_CHECK_DONE";
    case RADIO_CALIBRATION_BREAK: return "RADIO_CALIBRATION_BREAK";
    case RADIO_CALIBRATION_DONE: return "RADIO_CALIBRATION_DONE";
    case RADIO_CALIBRATION_SKIP: return "RADIO_CHECK_DONE";
    default:
        __RL_EXCEPTION__("UserUavEvent::toString:Unexpected event type");
    }
}

UserUavEventConnect::UserUavEventConnect(ICommAction::Type _connectionType, IAppCommInterface* _interface) :
    UserUavEvent(CONNECT),
    connetionType(_connectionType),
    interface(_interface)
{
}

ICommAction::Type UserUavEventConnect::getConnectionType(void) const
{
    return connetionType;
}

IAppCommInterface* UserUavEventConnect::getCommInnterface(void) const
{
    return interface;
}

UserUavEventAction::UserUavEventAction(ICommAction::Type _action) :
    UserUavEvent(ACTION),
    action(_action)
{
}

ICommAction::Type UserUavEventAction::getAction(void) const
{
    return action;
}

UserUavEventUpload::UserUavEventUpload(const ISignalPayloadMessage& _data) :
    UserUavEvent(UPLOAD),
    data(_data)
{
}

UserUavEventUpload::~UserUavEventUpload(void)
{
    delete &data;
}

const ISignalPayloadMessage& UserUavEventUpload::getData(void) const
{
    return data;
}

UserUavEventDownload::UserUavEventDownload(SignalData::Command _dataType) :
    UserUavEvent(DOWNLOAD),
    dataType(_dataType)
{
}

SignalData::Command UserUavEventDownload::getDataType(void) const
{
    return dataType;
}

UserUavEventAutopilot::UserUavEventAutopilot(const Vect2Dd& _position):
    UserUavEvent(AUTOPILOT),
    position(_position)
{
}

const Vect2Dd& UserUavEventAutopilot::getPosition(void) const
{
    return position;
}
