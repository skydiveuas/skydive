#include "ControlData.hpp"

#include <string.h>

#ifdef __SKYDIVE_USE_STL__

#include <iomanip>
#include <sstream>

#include "SkyException.hpp"

#endif //__SKYDIVE_USE_STL__

ControlData::ControlData(void)
{
    euler = Vect3Dd();
    throttle = 0.0f;
}

ControlData::ControlData(const unsigned char* src)
{
    memcpy((unsigned char*)this + 4, src, getDataSize());
}

IMessage::PreambleType ControlData::getPreambleType(void) const
{
    return IMessage::CONTROL;
}

void ControlData::serialize(unsigned char* dst) const
{
    memcpy(dst, (unsigned char*)this + 4, getDataSize());
}

IMessage::MessageType ControlData::getMessageType(void) const
{
    return CONTROL_DATA;
}

unsigned ControlData::getDataSize(void) const
{
    return sizeof(ControlData) - 4;
}

void ControlData::setEuler(const Vect3Df& _euler)
{
    euler = _euler;
}

void ControlData::setThrottle(const float _throttle)
{
    throttle = _throttle;
}

void ControlData::setControllerCommand(const ControllerCommand& _controllerCommand)
{
    controllerCommand = static_cast<unsigned short>(_controllerCommand);
}

void ControlData::setSolverMode(const SolverMode& _solverMode)
{
    solverMode = static_cast<unsigned char>(_solverMode);
}

const Vect3Df& ControlData::getEuler(void) const
{
    return euler;
}

float ControlData::getThrottle(void) const
{
    return throttle;
}

ControlData::ControllerCommand ControlData::getControllerCommand(void) const
{
    return static_cast<ControllerCommand>(controllerCommand);
}

ControlData::SolverMode ControlData::getSolverMode(void) const
{
    return static_cast<SolverMode>(solverMode);
}

void ControlData::setRoll(const float roll)
{
    euler.x = roll;
}

void ControlData::setPitch(const float pitch)
{
    euler.y = pitch;
}

void ControlData::setYaw(const float yaw)
{
    euler.z = yaw;
}

#ifdef __SKYDIVE_USE_STL__

std::string ControlData::getControllerCommandString(const ControllerCommand& controllerCommand)
{
    switch (controllerCommand)
    {
    case MANUAL:
        return std::string("Manual");
    case AUTOLANDING:
        return std::string("Autolanding");
    case AUTOLANDING_AP:
        return std::string("Autolanding AP");
    case HOLD_ALTITUDE:
        return std::string("Hold: altitude");
    case HOLD_POSITION:
        return std::string("Hold: position");
    case BACK_TO_BASE:
        return std::string("Back to base");
    case VIA_ROUTE:
        return std::string("Fly via route");
    case STOP:
        return std::string("STOP");
    case ERROR_JOYSTICK:
        return std::string("ERROR Joystick");
    default:
        return std::string("ERROR");
    }
}

ControlData::ControllerCommand ControlData::getCommandFromString(const std::string& str)
{
    if (str == getControllerCommandString(ControlData::MANUAL))
    {
        return ControlData::MANUAL;
    }
    else if (str == getControllerCommandString(ControlData::AUTOLANDING))
    {
        return ControlData::AUTOLANDING;
    }
    else if (str == getControllerCommandString(ControlData::AUTOLANDING_AP))
    {
        return ControlData::AUTOLANDING_AP;
    }
    else if (str == getControllerCommandString(ControlData::HOLD_ALTITUDE))
    {
        return ControlData::HOLD_ALTITUDE;
    }
    else if (str == getControllerCommandString(ControlData::HOLD_POSITION))
    {
        return ControlData::HOLD_POSITION;
    }
    else if (str == getControllerCommandString(ControlData::BACK_TO_BASE))
    {
        return ControlData::BACK_TO_BASE;
    }
    else if (str == getControllerCommandString(ControlData::VIA_ROUTE))
    {
        return ControlData::VIA_ROUTE;
    }
    else if (str == getControllerCommandString(ControlData::STOP))
    {
        return ControlData::STOP;
    }
    else if (str == getControllerCommandString(ControlData::ERROR_JOYSTICK))
    {
        return ControlData::ERROR_JOYSTICK;
    }
    else
    {
        __SKY_EXCEPTION__("Unexpected command string");
    }
}

std::string ControlData::getSolverModeString(const SolverMode& solverMode)
{
    switch (solverMode)
    {
    case STABLILIZATION:
        return std::string("Stabilization");
    case ANGLE_NO_YAW:
        return std::string("Angle no yaw");
    case ANGLE:
        return std::string("Angle");
    case HEADLESS:
        return std::string("Headless");
    default:
        return std::string("Error");
    }
}

ControlData::SolverMode ControlData::getSolverModeFromString(const std::string& str)
{
    if (str == getSolverModeString(ControlData::STABLILIZATION))
    {
        return ControlData::STABLILIZATION;
    }
    else if (str == getSolverModeString(ControlData::ANGLE_NO_YAW))
    {
        return ControlData::ANGLE_NO_YAW;
    }
    else if (str == getSolverModeString(ControlData::ANGLE))
    {
        return ControlData::ANGLE;
    }
    else if (str == getSolverModeString(ControlData::HEADLESS))
    {
        return ControlData::HEADLESS;
    }
    else
    {
        __SKY_EXCEPTION__("Unexpected command string");
    }
}

ControlData ControlData::parseFromString(const std::string& line)
{
    ControlData controlData;
    std::istringstream iss(line);
    float time;
    int solverMode;
    if (!(iss
          >> time
          >> controlData.euler.x >> controlData.euler.y >> controlData.euler.z
          >> controlData.throttle
          >> controlData.controllerCommand
          >> solverMode
          ))
    {
        __SKY_EXCEPTION__("Error while parsing data.");
    }
    controlData.solverMode = (unsigned char)solverMode;
    return controlData;
}

std::ostream& operator << (std::ostream& stream, const ControlData& controlData)
{
    stream << controlData.euler.x << " " << controlData.euler.y << " " << controlData.euler.z << " ";
    stream << controlData.throttle << " ";
    stream << controlData.controllerCommand << " ";
    stream << (int)controlData.solverMode << " ";
    return stream;
}

#endif //__SKYDIVE_USE_STL__
