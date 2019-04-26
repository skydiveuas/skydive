#include "communication/SensorsData.hpp"

#include <string.h>

SensorsData::SensorsData(void)
{
}

SensorsData::SensorsData(const unsigned char* src)
{
    memcpy((unsigned char*)this + 4, src, getDataSize());
}

IMessage::PreambleType SensorsData::getPreambleType(void) const
{
    return IMessage::CONTROL;
}

void SensorsData::serialize(unsigned char* dst) const
{
    memcpy(dst, (unsigned char*)this + 4, getDataSize());
}

IMessage::MessageType SensorsData::getMessageType(void) const
{
    return SENSORS_DATA;
}

unsigned SensorsData::getDataSize(void) const
{
    return sizeof(SensorsData) - 4;
}

void SensorsData::setGpsSpeed(const float _speed)
{
    speedGps = roboLib::valToShort(0.0f, MAX_GPS_SPEED, _speed);
}

void SensorsData::setGpsCourse(const float _course)
{
    courseGps = roboLib::valToShort(0.0f, MAX_GPS_COURSE, _course);
}

void SensorsData::setGpsAltitude(const float _altitude)
{
    altitudeGps = roboLib::valToShort(0.0f, MAX_GPS_ALTITUDE, _altitude);
}

void SensorsData::setGpsVerticalSpeed(const float _verticalSpeed)
{
    verticalSpeed = roboLib::valToShort(-MAX_GPS_V_SPEED, MAX_GPS_V_SPEED, _verticalSpeed);
}

void SensorsData::setGpsHdop(const float _hdop)
{
    hdop = roboLib::valToShort(-MAX_GPS_HDOP, MAX_GPS_HDOP, _hdop);
}

float SensorsData::getGpsSpeed(void) const
{
    return roboLib::shortToVal(0.0f, MAX_GPS_SPEED, speedGps);
}

float SensorsData::getGpsCourse(void) const
{
    return roboLib::shortToVal(0.0f, MAX_GPS_COURSE, courseGps);
}

float SensorsData::getGpsAltitude(void) const
{
    return roboLib::shortToVal(0.0f, MAX_GPS_ALTITUDE, altitudeGps);
}

float SensorsData::getGpsVerticalSpeed(void) const
{
    return roboLib::shortToVal(-MAX_GPS_V_SPEED, MAX_GPS_V_SPEED, verticalSpeed);
}

float SensorsData::getGpsHdop(void) const
{
	return roboLib::shortToVal(-MAX_GPS_HDOP, MAX_GPS_HDOP, hdop);
}

GpsData SensorsData::getGpsData(void) const
{
    GpsData gpsData;
    gpsData.lat = lat;
    gpsData.lon = lon;
    gpsData.speed = getGpsSpeed();
    gpsData.course = getGpsCourse();
    gpsData.alt = getGpsAltitude();
    gpsData.verticalSpeed = getGpsVerticalSpeed();
    gpsData.fix = (hdop >= 0.0f);
    if(gpsData.fix)
    {
    	gpsData.fixQuality = hdop < 2.0f ? GpsData::FIX_3D_DIFFERENTIAL : GpsData::FIX_2D_STAND_ALONE;
    }
    else
    {
    	gpsData.fixQuality = GpsData::FIX_NOT_AVAILABLE;
    }
    gpsData.HDOP = hdop;
    return gpsData;
}

ImuData SensorsData::getImuData(void) const
{
    ImuData imuData;
    imuData.omega = omega;
    imuData.accel = accel;
    imuData.magnet = magnet;
    imuData.pressure = pressure;
    return imuData;
}
