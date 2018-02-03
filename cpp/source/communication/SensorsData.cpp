#include "SensorsData.hpp"

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

GpsData SensorsData::getGpsData(void) const
{
    GpsData gpsData;
    gpsData.lat = lat;
    gpsData.lon = lon;
    gpsData.speed = speedGps;
    gpsData.course = courseGps;
    gpsData.alt = altitudeGps;
    gpsData.fix = (HDOP >= 0.0f);
    gpsData.HDOP = HDOP;
    return gpsData;
}

ImuData SensorsData::getImuData(void) const
{
    ImuData imuData;
    imuData.gyroRaw = gyroRaw;
    imuData.accelRaw = accelRaw;
    imuData.magnetRaw = magnetRaw;
    imuData.pressRaw = pressRaw;
    return imuData;
}
