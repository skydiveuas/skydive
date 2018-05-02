#include "common/Location.hpp"

Location::Location(void)
{
    position = getInvalidPosition();
    absoluteAltitude = getInvalidAltitude();
    relativeAltitude = getInvalidAltitude();
}

Location::Location(const Vect2Dd& _position, const float _absoluteAltitude, const float _relativeAltitude):
    position(_position),
    absoluteAltitude(_absoluteAltitude),
    relativeAltitude(_relativeAltitude)
{
}

bool Location::isValidPosition(void) const
{
    return position != getInvalidPosition();
}

bool Location::isValidAbsoluteAltitude(void) const
{
    return absoluteAltitude != getInvalidAltitude();
}

bool Location::isValidRelativeAltitude(void) const
{
    return relativeAltitude != getInvalidAltitude();
}

Vect2Df Location::getInvalidPosition(void)
{
    return Vect2Df(-100.0f, -100.0f);
}

float Location::getInvalidAltitude(void)
{
    return -100.0f;
}

Location Location::getInvalidLocation(void)
{
    return Location();
}
