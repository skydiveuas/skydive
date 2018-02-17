// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __SENSORS_DATA__
#define __SENSORS_DATA__

#include "MathCore.hpp"

#include "IMessage.hpp"

#include "ImuData.hpp"
#include "GpsData.hpp"

/**
 * =============================================================================================
 * SensorsData
 * =============================================================================================
 */
class SensorsData : public IMessage
{
public:
    float pressRaw;

    float lat, lon;
    float speedGps;
    float courseGps;
    float altitudeGps;
    float HDOP;

    Vect3Ds gyroRaw;
    Vect3Ds accelRaw;
    Vect3Ds magnetRaw;

    // 2 bytes left to use

    SensorsData(void);
    SensorsData(const unsigned char* tab);

    PreambleType getPreambleType(void) const;

    void serialize(unsigned char* tab) const;

    MessageType getMessageType(void) const;

    unsigned getDataSize(void) const;

    GpsData getGpsData(void) const;
    ImuData getImuData(void) const;
};

#endif // __SENSORS_DATA_PACKET__
