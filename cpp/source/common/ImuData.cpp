#include "common/ImuData.hpp"

#include <string.h>

#ifdef __SKYDIVE_USE_STL__

#include <stdexcept>
#include <iomanip>

#endif //__SKYDIVE_USE_STL__

ImuData::ImuData(void) 
{ 
}

#ifdef __SKYDIVE_USE_STL__

ImuData ImuData::parseFromString(const std::string& line)
{
    ImuData data;
    std::istringstream iss(line);
    float time;
    if (!(iss
          >> time
          >> data.omega.x >> data.omega.y >> data.omega.z
          >> data.accel.x >> data.accel.y >> data.accel.z
          >> data.magnet.x >> data.magnet.y >> data.magnet.z
          >> data.pressure
          ))
    {
        throw std::runtime_error("Error while parsing data.");
    }
    return data;
}

std::ostream& operator << (std::ostream& stream, const ImuData& sD)
{
    stream << sD.omega.x << "	" << sD.omega.y << "	" << sD.omega.z << "	";
    stream << sD.accel.x << "	" << sD.accel.y << "	" << sD.accel.z << "	";
    stream << sD.magnet.x << "	" << sD.magnet.y << "	" << sD.magnet.z << "	";
    stream << sD.pressure << "	";
    return stream;
}

#endif // __SKYDIVE_USE_STL__
