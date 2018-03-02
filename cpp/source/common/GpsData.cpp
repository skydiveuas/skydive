#include "GpsData.hpp"

#include <string.h>

#ifdef __SKYDIVE_USE_STL__

#include <iomanip>

#include "SkyException.hpp"

#endif //__SKYDIVE_USE_STL__

GpsData::GpsData(void)
{
}

GpsData::GpsData(const unsigned char* tab)
{
    memcpy(this, tab, sizeof(*this));
}

void GpsData::serialize(unsigned char* tab) const
{
    memcpy(tab, this, sizeof(*this));
}

#ifdef __SKYDIVE_USE_STL__

void GpsData::print(void) const
{
    if (fix)
    {
        std::cout << std::fixed;
        std::cout << std::setprecision(8) << lat << ", " << lon << " - ";
        std::cout << std::setprecision(1) << alt << "	|	";
        std::cout << std::setprecision(6) << speed << " @ ";
        std::cout << std::setprecision(2) << course << " deg" << std::endl;
    }
    else
    {
        std::cout << "no fix!" << std::endl;
    }
}

GpsData GpsData::parseFromString(const std::string& line)
{
    GpsData data;
    std::istringstream iss(line);
    int fix;
    float time;
    if (!(iss
          >> time
          >> fix
          >> data.lat >> data.lon >> data.alt
          >> data.speed >> data.course
          >> data.HDOP
          ))
    {
        __SKY_EXCEPTION__("Error while parsing data.");
    }
    data.fix = fix == 0 ? false : true;
    return data;
}

std::ostream& operator << (std::ostream& stream, const GpsData& gD)
{
    stream << std::setprecision(10);
    stream << (gD.fix ? 1 : 0) << "	" << gD.lat << "	" << gD.lon << "	" << gD.alt << "	";
    stream << gD.speed << "	" << gD.course << "	" << gD.HDOP << "	";
    return stream;
}

#endif // __SKYDIVE_USE_STL__
