// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __SENSORS_DATA__
#define __SENSORS_DATA__

#include "common/MathCore.hpp"

#include "IMessage.hpp"

#include "common/ImuData.hpp"
#include "common/GpsData.hpp"

/**
 * =============================================================================================
 * SensorsData
 * Max data size: 58 bytes
 * Used: 58 bytes
 * =============================================================================================
 */
class SensorsData : public IMessage
{
private:
    static constexpr float MAX_GPS_SPEED = 1000.0f;
    static constexpr float MAX_GPS_COURSE = 360.0f;
    static constexpr float MAX_GPS_ALTITUDE = 5000.0f;
    static constexpr float MAX_GPS_V_SPEED = 15.0f;
    static constexpr float MAX_GPS_HDOP = 5.0f;

public:
    // IMU
    float pressure;
    Vect3Df omega;
    Vect3Df accel;
    Vect3Df magnet;

    // GPS
    float lat, lon;
    unsigned short speedGps;
    unsigned short courseGps;
    unsigned short altitudeGps;
    unsigned short verticalSpeed;
    unsigned char fixQuality;
    unsigned char hdop;

    SensorsData(void);
    SensorsData(const unsigned char* tab);

    PreambleType getPreambleType(void) const;

    void serialize(unsigned char* tab) const;

    MessageType getMessageType(void) const;

    unsigned getDataSize(void) const;

    void setGpsSpeed(const float _speed);
    void setGpsCourse(const float _course);
    void setGpsAltitude(const float _altitude);
    void setGpsVerticalSpeed(const float _verticalSpeed);
    void setGpsHdop(const float _hdop);

    float getGpsSpeed(void) const;
    float getGpsCourse(void) const;
    float getGpsAltitude(void) const;
    float getGpsVerticalSpeed(void) const;
    float getGpsHdop(void) const;

    GpsData getGpsData(void) const;
    ImuData getImuData(void) const;
};

#endif // __SENSORS_DATA_PACKET__
