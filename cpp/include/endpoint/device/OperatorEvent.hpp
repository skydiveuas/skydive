#ifndef __USER_UAV_EVENT__
#define __USER_UAV_EVENT__

#include "ISignalPayloadMessage.hpp"

#include "IAppCommInterface.hpp"
#include "ICommAction.hpp"

#include "Exception.hpp"

#include <string>

class ICommAction;

/**
 * =============================================================================================
 * UserUavEvent
 * =============================================================================================
 */
class UserUavEvent
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

    UserUavEvent(const Type& _type);

    virtual ~UserUavEvent(void);
    Type getType(void) const;

    virtual std::string toString(void) const;
};

class UserUavEventConnect : public UserUavEvent
{
    ICommAction::Type connetionType;
    IAppCommInterface* interface;

public:
    UserUavEventConnect(ICommAction::Type _connectionType, IAppCommInterface* _interface);

    ICommAction::Type getConnectionType(void) const;

    IAppCommInterface* getCommInnterface(void) const;
};

class UserUavEventAction : public UserUavEvent
{
    ICommAction::Type action;

public:
    UserUavEventAction(ICommAction::Type _action);

    ICommAction::Type getAction(void) const;
};

class UserUavEventUpload : public UserUavEvent
{
    const ISignalPayloadMessage& data;

public:
    UserUavEventUpload(const ISignalPayloadMessage& _data);

    ~UserUavEventUpload(void);

    const ISignalPayloadMessage& getData(void) const;
};

class UserUavEventDownload : public UserUavEvent
{
    SignalData::Command dataType;

public:
    UserUavEventDownload(SignalData::Command _dataType);

    SignalData::Command getDataType(void) const;
};

class UserUavEventAutopilot : public UserUavEvent
{
    Vect2Dd position;

public:
    UserUavEventAutopilot(const Vect2Dd& _position);

    const Vect2Dd& getPosition(void) const;
};

#endif // __USER_UAV_EVENT__
