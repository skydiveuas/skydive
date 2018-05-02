#include "communication/RouteContainer.hpp"

#include <string.h>

#include "communication/IMessage.hpp"
#include "communication/SignalData.hpp"

RouteContainer::RouteContainer(void)
{
    route = NULL;
    constraint.routeSize = 0;
}

RouteContainer::RouteContainer(const unsigned char* src)
{
    // cast constraint of container
    memcpy(&constraint, src, getConstraintBinarySize());

    if (constraint.routeSize <= getMaxRouteSize())
    {
        // alocate memmory
        route = new Waypoint[constraint.routeSize];
        // cast dynamic route vector
        for (unsigned i = 0; i < constraint.routeSize; i++)
        {
            route[i] = Waypoint(
                        src + getConstraintBinarySize() + i * Waypoint::getDataSize());
        }
    }
    else
    {
        route = NULL;
        constraint.routeSize = 0;
    }
}

RouteContainer::RouteContainer(const RouteContainer& routeContainer)
{
    route = NULL;
    constraint.routeSize = 0;
    *this = routeContainer;
}

RouteContainer::RouteContainer(const Waypoint* const _route, const unsigned _routeSize,
                               const float _waypointTime, const float _baseTime)
{
    constraint.routeSize = _routeSize;
    constraint.waypointTime = _waypointTime;
    constraint.baseTime = _baseTime;

    route = new Waypoint[constraint.routeSize];
    for (unsigned i = 0; i < constraint.routeSize; i++)
    {
        route[i] = _route[i];
    }
    setCrc();
}

#ifdef __SKYDIVE_USE_STL__

RouteContainer::RouteContainer(const std::vector<Waypoint>& routeVector,
                               const float _waypointTime, const float _baseTime) :
    RouteContainer(routeVector.data(), routeVector.size(), _waypointTime, _baseTime)
{
}

#endif // __SKYDIVE_USE_STL__

void RouteContainer::serialize(unsigned char* dst) const
{
    // cast constraint of container
    memcpy(dst, &constraint, getConstraintBinarySize());
    // cast dynamic route vector
    for (unsigned i = 0; i < constraint.routeSize; i++)
    {
        route[i].serialize(
                    dst + getConstraintBinarySize() + i * Waypoint::getDataSize());
    }
}

unsigned RouteContainer::getDataSize() const
{
    return getBinarySize();
}

SignalData::Command RouteContainer::getSignalDataType(void) const
{
    return SignalData::ROUTE_CONTAINER_DATA;
}

SignalData::Command RouteContainer::getSignalDataCommand(void) const
{
    return SignalData::ROUTE_CONTAINER;
}

SignalData::Command RouteContainer::getUploadAction(void) const
{
    return SignalData::UPLOAD_ROUTE;
}

IMessage::MessageType RouteContainer::getMessageType(void) const
{
    return ROUTE_CONTAINER;
}

bool RouteContainer::isValid(void) const
{
    unsigned char *dataTab = new unsigned char[getBinarySize()];
    serialize(dataTab);
    bool result = (IMessage::computeCrc32(dataTab + 4, getBinarySize() - 4) == constraint.crcValue);
    delete[] dataTab;
    return result;
}

unsigned RouteContainer::getCrc(void) const
{
    return constraint.crcValue;
}

void RouteContainer::setCrc(void)
{
    unsigned char *dataTab = new unsigned char[getBinarySize()];
    serialize(dataTab);
    constraint.crcValue = IMessage::computeCrc32(dataTab + 4, getBinarySize() - 4);
    delete[] dataTab;
}

ISignalPayloadMessage* RouteContainer::clone(void) const
{
    return new RouteContainer(*this);
}

unsigned RouteContainer::getRouteBinarySize(void) const
{
    return Waypoint::getDataSize() * constraint.routeSize;
}

unsigned RouteContainer::getBinarySize(void) const
{
    return getConstraintBinarySize() + getRouteBinarySize();
}

const Waypoint* RouteContainer::getRouteWaypoint(const unsigned waypointIndex) const
{
    if (waypointIndex < constraint.routeSize)
    {
        // if index is in boundry, return pointed Location
        return route + waypointIndex;
    }
    else
    {
        // if invalid index was sepcified, return last Location in route
        return route + (constraint.routeSize - 1);
    }
}

unsigned RouteContainer::getRouteSize(void) const
{
    return constraint.routeSize;
}

float RouteContainer::getWaypointTime(void) const
{
    return constraint.waypointTime;
}

float RouteContainer::getBaseTime(void) const
{
    return constraint.baseTime;
}

unsigned RouteContainer::getWaypointStablilizationMaxCounter(const float dt) const
{
    return (unsigned)(constraint.waypointTime / dt);
}

unsigned RouteContainer::getBaseStablilizationMaxCounter(const float dt) const
{
    return (unsigned)(constraint.baseTime / dt);
}

bool RouteContainer::isRouteEnded(const unsigned waypointIndex) const
{
    return (waypointIndex + 1) >= constraint.routeSize;
}

bool RouteContainer::isContainerFilled(void) const
{
    return constraint.routeSize != 0 && constraint.routeSize <= getMaxRouteSize();
}

RouteContainer& RouteContainer::operator=(const RouteContainer& right)
{
    if (route != NULL)
    {
        delete[] route;
    }

    if (right.isContainerFilled())
    {
        constraint.routeSize = right.constraint.routeSize;
        route = new Waypoint[constraint.routeSize];
        for (unsigned i = 0; i < constraint.routeSize; i++)
        {
            route[i] = right.route[i];
        }
    }

    constraint.waypointTime = right.constraint.waypointTime;
    constraint.baseTime = right.constraint.baseTime;
    constraint.crcValue = right.constraint.crcValue;

    return *this;
}

RouteContainer::~RouteContainer(void)
{
    if (route != NULL)
    {
        delete[] route;
    }
}

unsigned RouteContainer::getConstraintBinarySize(void)
{
    return sizeof(Constraint);
}

unsigned RouteContainer::getMaxRouteSize(void)
{
    return 16;
}

unsigned RouteContainer::getMaxRouteContainerBinarySize(void)
{
    return getConstraintBinarySize() + Waypoint::getDataSize() * getMaxRouteSize();
}
