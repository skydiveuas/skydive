// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __AUTOPILOT_DATA__
#define __AUTOPILOT_DATA__

#include "MathCore.hpp"

#include "IMessage.hpp"

#include "Location.hpp"
#include "Flags.hpp"

/**
 * =============================================================================================
 * AutopilotData
 * =============================================================================================
 */
class AutopilotData : public IMessage
{
public:
    enum Type
    {
        INVALID_TYPE = 1000,
        BASE = 2000, // from UAV
        BASE_ACK = 2100,
        TARGET = 3000, // from operator
        TARGET_ACK = 3100,
        TARGET_NOT_ALLOWED_STATE = 3200,
        TARGET_NOT_ALLOWED_SETTINGS = 3300
    };

    enum FlagId
    {
        ALTITUDE_DEFINED,
        AUTOLAND_AT_TARGET,
        RETRANSMITED,
    };

    AutopilotData(void);
    AutopilotData(const Type t, const Location& _location);
    AutopilotData(const unsigned char* src);

    PreambleType getPreambleType(void) const;

    void serialize(unsigned char* dst) const;

    MessageType getMessageType(void) const;

    unsigned getDataSize() const;

    const Location& getLocation() const;

    const Vect2Dd& getTargetPosition(void) const;
    float getTargetAltitude(void) const;
    float getAbsoluteAltitude() const;

    void setTargetPosition(const Vect2Dd& _targetPosition);
    void setTargetAltitude(const float _targetAltitude);
    void setAbsoluteAltitude(const float _absoluteAltitude);

    Type getType() const;
    void setType(const Type t);

    const Flags<int>& flags(void) const;
    Flags<int>& flags(void);

#ifdef __SKYDIVE_USE_STL__

    static AutopilotData parseFromString(const std::string& line);
    friend std::ostream& operator << (std::ostream& stream, const AutopilotData& autopilotData);

    std::string getTypeString() const;
    static std::string getTypeString(const Type type);

#endif //__SKYDIVE_USE_STL__

private:
    Location location;

    int type;

    Flags<int> flagsObj;
};

#endif // __AUTOPILOT_DATA__
