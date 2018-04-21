// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#include "PilotEvent.hpp"

PilotEvent::PilotEvent(const Type& _type) :
    type(_type)
{
}

PilotEvent::~PilotEvent(void)
{
}

PilotEvent::Type PilotEvent::getType(void) const
{
    return type;
}

std::string PilotEvent::toString(void) const
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

PilotEventConnect::PilotEventConnect(ISkyDeviceAction::Type _connectionType, ISkyCommInterface* _interface) :
    PilotEvent(CONNECT),
    connetionType(_connectionType),
    interface(_interface)
{
}

ISkyDeviceAction::Type PilotEventConnect::getConnectionType(void) const
{
    return connetionType;
}

ISkyCommInterface* PilotEventConnect::getCommInnterface(void) const
{
    return interface;
}

PilotEventAction::PilotEventAction(ISkyDeviceAction::Type _action) :
    PilotEvent(ACTION),
    action(_action)
{
}

ISkyDeviceAction::Type PilotEventAction::getAction(void) const
{
    return action;
}

PilotEventUpload::PilotEventUpload(const ISignalPayloadMessage& _data) :
    PilotEvent(UPLOAD),
    data(_data)
{
}

PilotEventUpload::~PilotEventUpload(void)
{
    delete &data;
}

const ISignalPayloadMessage& PilotEventUpload::getData(void) const
{
    return data;
}

PilotEventDownload::PilotEventDownload(SignalData::Command _dataType) :
    PilotEvent(DOWNLOAD),
    dataType(_dataType)
{
}

SignalData::Command PilotEventDownload::getDataType(void) const
{
    return dataType;
}

PilotEventAutopilot::PilotEventAutopilot(const Vect2Dd& _position):
    PilotEvent(AUTOPILOT),
    position(_position)
{
}

const Vect2Dd& PilotEventAutopilot::getPosition(void) const
{
    return position;
}
