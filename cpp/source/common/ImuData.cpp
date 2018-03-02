#include "ImuData.hpp"

#include <string.h>

#ifdef __SKYDIVE_USE_STL__

#include <iomanip>

#include "SkyException.hpp"

#endif //__SKYDIVE_USE_STL__

ImuData::ImuData(void) 
{ 
}

ImuData::ImuData(const unsigned char* tab)
{
    memcpy(this, tab, sizeof(*this));
}

void ImuData::serialize(unsigned char* tab) const
{
    memcpy(tab, this, sizeof(*this));
}

#ifdef __SKYDIVE_USE_STL__

ImuData ImuData::parseFromString(const std::string& line)
{
    ImuData data;
    std::istringstream iss(line);
    float time;
    if (!(iss
          >> time
          >> data.gyroRaw.x >> data.gyroRaw.y >> data.gyroRaw.z
          >> data.accelRaw.x >> data.accelRaw.y >> data.accelRaw.z
          >> data.magnetRaw.x >> data.magnetRaw.y >> data.magnetRaw.z
          >> data.pressRaw
          ))
    {
        __SKY_EXCEPTION__("Error while parsing data.");
    }
    return data;
}

std::ostream& operator << (std::ostream& stream, const ImuData& sD)
{
    stream << sD.gyroRaw.x << "	" << sD.gyroRaw.y << "	" << sD.gyroRaw.z << "	";
    stream << sD.accelRaw.x << "	" << sD.accelRaw.y << "	" << sD.accelRaw.z << "	";
    stream << sD.magnetRaw.x << "	" << sD.magnetRaw.y << "	" << sD.magnetRaw.z << "	";
    stream << sD.pressRaw << "	";
    return stream;
}

#endif // __SKYDIVE_USE_STL__
