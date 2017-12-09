// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __WAYPOINT__
#define __WAYPOINT__

#include "Location.hpp"

/**
 * =============================================================================================
 * Waypoint
 * =============================================================================================
 */
class Waypoint
{
public:
	Location location;
	float velocity;

	Waypoint(void);
	Waypoint(const unsigned char* src);
	Waypoint(const Location& _location, const float _velocity);

	void serialize(unsigned char* dst) const;

	static unsigned getDataSize(void);
};

#endif // __WAYPOINT__
