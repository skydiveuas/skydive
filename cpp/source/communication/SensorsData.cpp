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
    verticalSpeed = roboLib::valToShort(0.0f, MAX_GPS_V_SPEED, _verticalSpeed);
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
    return roboLib::shortToVal(0.0f, MAX_GPS_V_SPEED, verticalSpeed);
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
    gpsData.fix = (HDOP >= 0.0f);
    gpsData.HDOP = HDOP;
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
