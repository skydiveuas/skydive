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
 * UserUavEvent
 * =============================================================================================
 */
class OperatorEvent
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

    OperatorEvent(const Type& _type);

    virtual ~OperatorEvent(void);
    Type getType(void) const;

    virtual std::string toString(void) const;
};

class OperatorEventConnect : public OperatorEvent
{
    ISkyDeviceAction::Type connetionType;
    ISkyCommInterface* interface;

public:
    OperatorEventConnect(ISkyDeviceAction::Type _connectionType, ISkyCommInterface* _interface);

    ISkyDeviceAction::Type getConnectionType(void) const;

    ISkyCommInterface* getCommInnterface(void) const;
};

class UserUavEventAction : public OperatorEvent
{
    ISkyDeviceAction::Type action;

public:
    UserUavEventAction(ISkyDeviceAction::Type _action);

    ISkyDeviceAction::Type getAction(void) const;
};

class OperatorEventUpload : public OperatorEvent
{
    const ISignalPayloadMessage& data;

public:
    OperatorEventUpload(const ISignalPayloadMessage& _data);

    ~OperatorEventUpload(void);

    const ISignalPayloadMessage& getData(void) const;
};

class OperatorEventDownload : public OperatorEvent
{
    SignalData::Command dataType;

public:
    OperatorEventDownload(SignalData::Command _dataType);

    SignalData::Command getDataType(void) const;
};

class OperatorEventAutopilot : public OperatorEvent
{
    Vect2Dd position;

public:
    OperatorEventAutopilot(const Vect2Dd& _position);

    const Vect2Dd& getPosition(void) const;
};

#endif // __USER_UAV_EVENT__
