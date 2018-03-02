#include "AutopilotData.hpp"

#include <string.h>

#ifdef __SKYDIVE_USE_STL__

#include <iomanip>
#include <sstream>

#include "SkyException.hpp"

#endif //__SKYDIVE_USE_STL__

AutopilotData::AutopilotData(void):
    type(INVALID_TYPE)
{
}

AutopilotData::AutopilotData(const Type t, const Location& _location):
    location(_location),
    type(t)
{
}

AutopilotData::AutopilotData(const unsigned char* src)
{
    // pointer is offseted because of vptr and padding for double
    memcpy((unsigned char*)this + 8, src, getDataSize());
}

IMessage::PreambleType AutopilotData::getPreambleType(void) const
{
    return IMessage::AUTOPILOT;
}

void AutopilotData::serialize(unsigned char* dst) const
{
    // pointer is offseted because of vptr and padding for double
    memcpy(dst, (unsigned char*)this + 8, getDataSize());
}

IMessage::MessageType AutopilotData::getMessageType(void) const
{
    return AUTOPILOT_DATA;
}

unsigned AutopilotData::getDataSize(void) const
{
    // size is lower because od vptr and 8 byte padding (double in position)
    return sizeof(AutopilotData) - 8;
}

const Location& AutopilotData::getLocation() const
{
    return location;
}

const Vect2Dd& AutopilotData::getTargetPosition(void) const
{
    return location.position;
}

float AutopilotData::getTargetAltitude(void) const
{
    return location.relativeAltitude;
}

float AutopilotData::getAbsoluteAltitude() const
{
    return location.absoluteAltitude;
}

void AutopilotData::setTargetPosition(const Vect2Dd& _targetPosition)
{
    location.position = _targetPosition;
}

void AutopilotData::setTargetAltitude(const float _targetAltitude)
{
    location.relativeAltitude = _targetAltitude;
}

void AutopilotData::setAbsoluteAltitude(const float _absoluteAltitude)
{
    location.absoluteAltitude = _absoluteAltitude;
}

AutopilotData::Type AutopilotData::getType() const
{
    return static_cast<Type>(type);
}

void AutopilotData::setType(const Type t)
{
    type = static_cast<int>(t);
}

const Flags<int>& AutopilotData::flags(void) const
{
    return flagsObj;
}

Flags<int>& AutopilotData::flags(void)
{
    return flagsObj;
}

#ifdef __SKYDIVE_USE_STL__

AutopilotData AutopilotData::parseFromString(const std::string&)
{
    __SKY_EXCEPTION__("AutopilotData::parseFromString not implmented yet!");
}

std::ostream& operator << (std::ostream& stream, const AutopilotData& autopilotData)
{
    stream << std::setprecision(10);
    stream << autopilotData.location.position.x << " ";
    stream << autopilotData.location.position.y << " ";
    stream << autopilotData.location.absoluteAltitude << " ";
    stream << autopilotData.location.relativeAltitude << " ";
    stream << autopilotData.type << " ";
    stream << autopilotData.flags().getFlagsVector() << " ";
    return stream;
}

std::string AutopilotData::getTypeString() const
{
    return getTypeString(getType());
}

std::string AutopilotData::getTypeString(const Type type)
{
    switch (type)
    {
    case INVALID_TYPE: return "INVALID_TYPE";
    case BASE: return "BASE";
    case BASE_ACK: return "BASE_ACK";
    case TARGET: return "TARGET";
    case TARGET_ACK: return "TARGET_ACK";
    case TARGET_NOT_ALLOWED_STATE: return "TARGET_NOT_ALLOWED_STATE";
    case TARGET_NOT_ALLOWED_SETTINGS: return "TARGET_NOT_ALLOWED_SETTINGS";
    default:
        __SKY_EXCEPTION__("Unxpected type");
    }
}

#endif //__SKYDIVE_USE_STL__
