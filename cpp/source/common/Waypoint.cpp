#include "Waypoint.hpp"

#include <cstring>

Waypoint::Waypoint(void) :
location(), velocity(0.0f)
{
}

Waypoint::Waypoint(const Location& _location, const float _velocity) :
location(_location), velocity(_velocity)
{
}

Waypoint::Waypoint(const unsigned char* src)
{
	memcpy((unsigned char*)this, src, getDataSize());
}

void Waypoint::serialize(unsigned char* dst) const
{
	memcpy(dst, (unsigned char*)this, getDataSize());
}

unsigned Waypoint::getDataSize(void)
{
	// WARING!!! - size is lower because of dummy padding
	// accrding to 8 bytes double padding from Location
	return sizeof(Waypoint) - 4;
}
