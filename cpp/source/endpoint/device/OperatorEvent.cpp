// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#include "OperatorEvent.hpp"

OperatorEvent::OperatorEvent(const Type& _type) :
    type(_type)
{
}

OperatorEvent::~OperatorEvent(void)
{
}

OperatorEvent::Type OperatorEvent::getType(void) const
{
    return type;
}

std::string OperatorEvent::toString(void) const
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
        __SKY_EXCEPTION__("UserUavEvent::toString:Unexpected event type");
    }
}

OperatorEventConnect::OperatorEventConnect(ISkyDeviceAction::Type _connectionType, ISkyCommInterface* _interface) :
    OperatorEvent(CONNECT),
    connetionType(_connectionType),
    interface(_interface)
{
}

ISkyDeviceAction::Type OperatorEventConnect::getConnectionType(void) const
{
    return connetionType;
}

ISkyCommInterface* OperatorEventConnect::getCommInnterface(void) const
{
    return interface;
}

UserUavEventAction::UserUavEventAction(ISkyDeviceAction::Type _action) :
    OperatorEvent(ACTION),
    action(_action)
{
}

ISkyDeviceAction::Type UserUavEventAction::getAction(void) const
{
    return action;
}

OperatorEventUpload::OperatorEventUpload(const ISignalPayloadMessage& _data) :
    OperatorEvent(UPLOAD),
    data(_data)
{
}

OperatorEventUpload::~OperatorEventUpload(void)
{
    delete &data;
}

const ISignalPayloadMessage& OperatorEventUpload::getData(void) const
{
    return data;
}

OperatorEventDownload::OperatorEventDownload(SignalData::Command _dataType) :
    OperatorEvent(DOWNLOAD),
    dataType(_dataType)
{
}

SignalData::Command OperatorEventDownload::getDataType(void) const
{
    return dataType;
}

OperatorEventAutopilot::OperatorEventAutopilot(const Vect2Dd& _position):
    OperatorEvent(AUTOPILOT),
    position(_position)
{
}

const Vect2Dd& OperatorEventAutopilot::getPosition(void) const
{
    return position;
}
