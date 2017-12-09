// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __ROUTE_CONTAINER__
#define __ROUTE_CONTAINER__

#ifdef __SKYDIVE_USE_STL__

#include <vector>

#endif // __SKYDIVE_USE_STL__

#include "ISignalPayloadMessage.hpp"

#include "SignalData.hpp"
#include "Waypoint.hpp"

/**
 * =============================================================================================
 * RouteContainer
 * =============================================================================================
 */
class RouteContainer : public ISignalPayloadMessage
{
public:
    struct Constraint
    {
        unsigned crcValue;
        unsigned routeSize;
        float waypointTime; // [s]
        float baseTime; // [s]
    };

    Constraint constraint;
    Waypoint* route;

    RouteContainer(void);
    RouteContainer(const unsigned char* src);
    RouteContainer(const RouteContainer& routeContainer);
    RouteContainer(const Waypoint* const _route, const unsigned _routeSize,
                   const float _waypointTime, const float _baseTime);

#ifdef __SKYDIVE_USE_STL__

    RouteContainer(const std::vector<Waypoint>& routeVector,
                   const float _waypointTime, const float _baseTime);

#endif // __SKYDIVE_USE_STL__

    void serialize(unsigned char* dst) const;

    unsigned getDataSize() const;

    SignalData::Command getSignalDataType(void) const;
    SignalData::Command getSignalDataCommand(void) const;
    SignalData::Command getUploadAction(void) const;

    MessageType getMessageType(void) const;

    bool isValid(void) const;

    unsigned getCrc(void) const;

    void setCrc(void);

    ISignalPayloadMessage* clone(void) const;

    unsigned getRouteBinarySize(void) const;

    unsigned getBinarySize(void) const;

    const Waypoint* getRouteWaypoint(const unsigned waypointIndex) const;

    unsigned getRouteSize(void) const;

    float getWaypointTime(void) const;

    float getBaseTime(void) const;

    unsigned getWaypointStablilizationMaxCounter(const float dt) const;

    unsigned getBaseStablilizationMaxCounter(const float dt) const;

    bool isRouteEnded(const unsigned waypointIndex) const;

    bool isContainerFilled(void) const;

    RouteContainer& operator=(const RouteContainer& right);

    ~RouteContainer(void);

    static unsigned getConstraintBinarySize(void);
    static unsigned getMaxRouteSize(void);
    static unsigned getMaxRouteContainerBinarySize(void);
};

#endif // __ROUTE_CONTAINER__
