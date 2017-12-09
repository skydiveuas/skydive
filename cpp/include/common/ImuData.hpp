// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __IMU_DATA__
#define __IMU_DATA__

#ifdef __SKYDIVE_USE_STL__

#include <iostream>
#include <sstream>

#endif //__SKYDIVE_USE_STL__

#include "MathCore.hpp"

/**
 * =============================================================================================
 * ImuData
 * =============================================================================================
 */
class ImuData
{
public:
	Vect3Ds gyroRaw;
	Vect3Ds accelRaw;
	Vect3Ds magnetRaw;
	float pressRaw;

	ImuData(void);
	ImuData(const unsigned char* tab);

	void serialize(unsigned char* tab) const;

#ifdef __SKYDIVE_USE_STL__

	static ImuData parseFromString(const std::string& line);
	friend std::ostream& operator << (std::ostream& stream, const ImuData& sD);

#endif // __SKYDIVE_USE_STL__

};

#endif // __IMU_DATA__
