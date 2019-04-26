#include "common/GpsData.hpp"

#include <string.h>

#ifdef __SKYDIVE_USE_STL__

#include <iomanip>

#endif // __SKYDIVE_USE_STL__

GpsData::GpsData(void)
    : fix(false),
      justFixed(false),
      justFixLost(false)
{
}

Vect2Dd GpsData::getSpeedVector() const
{
    return {std::cos(roboLib::toRad(course))*speed,
                std::sin(roboLib::toRad(course))*speed};
}

Vect2Dd GpsData::getGeoPoint() const
{
    return {lat, lon};
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
        std::cout << std::setprecision(2) << course << " deg, vh ";
        std::cout << std::setprecision(6) << verticalSpeed << std::endl;
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
          >> data.HDOP >> data.verticalSpeed
          ))
    {
        throw std::runtime_error("Error while parsing data.");
    }
    data.fix = fix == 0 ? false : true;
    return data;
}

std::ostream& operator << (std::ostream& stream, const GpsData& gD)
{
    stream << std::setprecision(10);
    stream << (gD.fix ? 1 : 0) << "	" << gD.lat << "	" << gD.lon << "	" << gD.alt << "	";
    stream << gD.speed << "	" << gD.course << "	" << gD.HDOP << "	" << gD.verticalSpeed << "  ";
    return stream;
}

#endif // __SKYDIVE_USE_STL__
