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

/**
 * =============================================================================================
 * GpsData
 * =============================================================================================
 */
class GpsData
{
public:
	double lat, lon; // polozenie
	float alt; // wysokosc
	float speed, course; // predkosc
	float HDOP; // dokladnosc
	bool fix;

	GpsData(void);
	GpsData(const unsigned char* tab);

	void serialize(unsigned char* tab) const;

#ifdef __SKYDIVE_USE_STL__

	void print(void) const;
	static GpsData parseFromString(const std::string& line);
	friend std::ostream& operator << (std::ostream& stream, const GpsData& gD);

#endif // __SKYDIVE_USE_STL__

};

#endif // __GPS_DATA__
