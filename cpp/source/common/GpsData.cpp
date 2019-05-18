#include "common/GpsData.hpp"

#include <string.h>

#ifdef __SKYDIVE_USE_STL__

#include <iomanip>

#endif // __SKYDIVE_USE_STL__

GpsData::GpsData(void)
: lat(0.0),
  lon(0.0),
  alt(0.0),
  speed(0.0),
  course(0.0),
  verticalSpeed(0.0),
  HDOP(0.0),
  VDOP(0.0),
  TDOP(0.0),
  hAcc(0.0),
  vAcc(0.0),
  satellitesNum(0),
  fix(false),
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

bool GpsData::is3dFix() const
{
	if(fixQuality == GpsData::FIX_3D_STAND_ALONE
			|| fixQuality == GpsData::FIX_3D_DIFFERENTIAL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#ifdef __SKYDIVE_USE_STL__

void GpsData::print(void) const
{
    if (fix)
    {
        std::cout << std::fixed;
        std::cout << std::setprecision(8) << lat << ", " << lon << " - ";
        std::cout << std::setprecision(3) << alt << "[m] | ";
        std::cout << std::setprecision(4) << speed << "[m/s] @ ";
        std::cout << std::setprecision(2) << course << " deg, vSpeed: ";
        std::cout << std::setprecision(4) << verticalSpeed << "[m/s], HDOP:  ";
        std::cout << std::setprecision(2) << HDOP << " fixQ: ";
        std::cout << (int)fixQuality << std::endl;
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
    int fixQ;
    if (!(iss
          >> time
          >> fix
          >> data.lat >> data.lon >> data.alt
          >> data.speed >> data.course >> data.verticalSpeed
          >> data.HDOP >> fixQ
          ))
    {
        throw std::runtime_error("Error while parsing data.");
    }
    data.fix = fix == 0 ? false : true;
    data.fixQuality = (uint8_t)fixQ;
    return data;
}

std::ostream& operator << (std::ostream& stream, const GpsData& gD)
{
    stream << std::setprecision(10);
    stream << (gD.fix ? 1 : 0) << '\t' << gD.lat << '\t' << gD.lon << '\t' << gD.alt << '\t';
    stream << gD.speed << '\t' << gD.course << '\t' << gD.verticalSpeed  << '\t';
    stream << gD.HDOP << '\t' << gD.fixQuality << '\t';
    return stream;
}

#endif // __SKYDIVE_USE_STL__
