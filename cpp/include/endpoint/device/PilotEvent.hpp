#ifndef __USER_UAV_EVENT__
#define __USER_UAV_EVENT__

#include "ISignalPayloadMessage.hpp"

#include "ISkyCommInterface.hpp"
#include "ISkyDeviceAction.hpp"

#include "SkyException.hpp"

#include <string>

class ISkyDeviceAction;

/**
 * =============================================================================================
 * PilotEvent
 * =============================================================================================
 */
class PilotEvent
{
public:
    enum Type
    {
        CONNECT,
        ACTION,
        UPLOAD,
        DOWNLOAD,
        AUTOPILOT,
        MAGNET_CALIB_DONE,
        MAGNET_CALIB_ABORT,
        BREAK_APPLICATION_LOOP,
        BREAK_FLIGHT_LOOP,
        BREAK_RADIO_CHECK,
        BREAK_SENSORS_LOGGER,
        ESC_CALIB_DONE,
        ESC_CALIB_ABORT,
        RADIO_CHECK_DONE,
        RADIO_CALIBRATION_BREAK,
        RADIO_CALIBRATION_DONE,
        RADIO_CALIBRATION_SKIP
    };

    const Type type;

    PilotEvent(const Type& _type);

    virtual ~PilotEvent(void);
    Type getType(void) const;

    virtual std::string toString(void) const;
};

class PilotEventConnect : public PilotEvent
{
    ISkyDeviceAction::Type connetionType;
    ISkyCommInterface* interface;

public:
    PilotEventConnect(ISkyDeviceAction::Type _connectionType, ISkyCommInterface* _interface);

    ISkyDeviceAction::Type getConnectionType(void) const;

    ISkyCommInterface* getCommInnterface(void) const;
};

class PilotEventAction : public PilotEvent
{
    ISkyDeviceAction::Type action;

public:
    PilotEventAction(ISkyDeviceAction::Type _action);

    ISkyDeviceAction::Type getAction(void) const;
};

class PilotEventUpload : public PilotEvent
{
    const ISignalPayloadMessage& data;

public:
    PilotEventUpload(const ISignalPayloadMessage& _data);

    ~PilotEventUpload(void);

    const ISignalPayloadMessage& getData(void) const;
};

class PilotEventDownload : public PilotEvent
{
    SignalData::Command dataType;

public:
    PilotEventDownload(SignalData::Command _dataType);

    SignalData::Command getDataType(void) const;
};

class PilotEventAutopilot : public PilotEvent
{
    Vect2Dd position;

public:
    PilotEventAutopilot(const Vect2Dd& _position);

    const Vect2Dd& getPosition(void) const;
};

#endif // __USER_UAV_EVENT__
