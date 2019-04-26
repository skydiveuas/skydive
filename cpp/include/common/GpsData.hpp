// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __GPS_DATA__
#define __GPS_DATA__

#ifdef __SKYDIVE_USE_STL__

#include <iostream>
#include <sstream>

#endif //__SKYDIVE_USE_STL__

#include "MathCore.hpp"
#include <cstdint>

/**
 * =============================================================================================
 * GpsData
 * =============================================================================================
 */
class GpsData
{
public:
    static constexpr uint8_t FIX_NOT_AVAILABLE = 0;
    static constexpr uint8_t FIX_2D_STAND_ALONE = 1;
    static constexpr uint8_t FIX_3D_STAND_ALONE = 2;
    static constexpr uint8_t FIX_2D_DIFFERENTIAL = 3;
    static constexpr uint8_t FIX_3D_DIFFERENTIAL = 4;
    static constexpr uint8_t FIX_TYPE_NOT_SUPPORTED = UINT8_MAX;

    double lat, lon;
    float alt;
    float speed, course;
    float verticalSpeed;
    float HDOP;
    float VDOP;
    float TDOP;
    float hAcc;
    float vAcc;
    uint8_t satellitesNum;
    bool fix, justFixed, justFixLost;
    uint8_t fixQuality;

    GpsData(void);

    Vect2Dd getSpeedVector() const;
    Vect2Dd getGeoPoint() const;
    bool is3dFix() const;

#ifdef __SKYDIVE_USE_STL__

    void print(void) const;
    static GpsData parseFromString(const std::string& line);
    friend std::ostream& operator << (std::ostream& stream, const GpsData& gD);

#endif // __SKYDIVE_USE_STL__

};

#endif // __GPS_DATA__
