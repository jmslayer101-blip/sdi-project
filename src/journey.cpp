#include "journey.h"

namespace GPS
{

Journey::Journey(std::vector<TimedWaypoint> timedWaypoints) : points{timedWaypoints} {}


// TODO: Stub definition needs implementing
unsigned int Journey::numberOfWaypoints() const
{
    return 0;
}

// TODO: Stub definition needs implementing
seconds Journey::totalTime() const
{
    return 0;
}

// TODO: Stub definition needs implementing
metres Journey::netHeightGain() const
{
    return 0;
}

// TODO: Stub definition needs implementing
metres Journey::totalHeightGain() const
{
    return 0;
}

// TODO: Stub definition needs implementing
metres Journey::netLength() const
{
    return 0;
}

// TODO: Stub definition needs implementing
metres Journey::totalLength() const
{
    return 0;
}

// TODO: Stub definition needs implementing
metresPerSecond Journey::averageSpeed() const
{
    return 0;
}

// TODO: Stub definition needs implementing
metresPerSecond Journey::maxSpeed() const
{
    return 0;
}

// TODO: Stub definition needs implementing
Waypoint Journey::highestWaypoint() const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
Waypoint Journey::lowestWaypoint() const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
Waypoint Journey::mostNorthelyWaypoint() const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
Waypoint Journey::mostSoutherlyWaypoint() const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
Waypoint Journey::mostEasterlyWaypoint() const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
Waypoint Journey::mostWesterlyWaypoint() const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
Waypoint Journey::mostEquatorialWaypoint() const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
Waypoint Journey::leastEquatorialWaypoint() const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
metresPerSecond Journey::maxRateOfAscent() const
{
    return 0;
}

// TODO: Stub definition needs implementing
metresPerSecond Journey::maxRateOfDescent() const
{
    return 0;
}

// TODO: Stub definition needs implementing
degrees Journey::maxGradient(metres) const
{
    return 0;
}

// TODO: Stub definition needs implementing
degrees Journey::minGradient(metres) const
{
    return 0;
}

// TODO: Stub definition needs implementing
degrees Journey::steepestGradient(metres) const
{
    return 0;
}

// TODO: Stub definition needs implementing
Waypoint Journey::nearestWaypointTo(Waypoint) const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
Waypoint Journey::farthestWaypointFrom(Waypoint) const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
fraction Journey::proportionOfWaypointsNear(Waypoint, metres) const
{
    return 0;
}

// TODO: Stub definition needs implementing
Waypoint Journey::lastWaypointBefore(std::time_t) const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
Waypoint Journey::firstWaypointAfter(std::time_t) const
{
    return Waypoint(0,0,0);
}

// TODO: Stub definition needs implementing
fraction Journey::proportionOfTimeSteeperThan(degrees) const
{
    return 0;
}

// TODO: Stub definition needs implementing
fraction Journey::proportionOfTimeAscendingSteeperThan(degrees) const
{
    return 0;
}

// TODO: Stub definition needs implementing
fraction Journey::proportionOfTimeDescendingSteeperThan(degrees) const
{
    return 0;
}

// TODO: Stub definition needs implementing
seconds Journey::restingTime(metresPerSecond) const
{
    return 0;
}

// TODO: Stub definition needs implementing
seconds Journey::travellingTime(metresPerSecond) const
{
    return 0;
}

// TODO: Stub definition needs implementing
seconds Journey::longestRestingPeriod(metresPerSecond) const
{
    return 0;
}

// TODO: Stub definition needs implementing
seconds Journey::longestTravellingPeriod(metresPerSecond) const
{
    return 0;
}

// TODO: Stub definition needs implementing
seconds Journey::averageRestingPeriod(metresPerSecond) const
{
    return 0;
}

// TODO: Stub definition needs implementing
seconds Journey::averageTravellingPeriod(metresPerSecond) const
{
    return 0;
}

// TODO: Stub definition needs implementing
fraction Journey::proportionRestingTime(metresPerSecond) const
{
    return 0;
}

// TODO: Stub definition needs implementing
fraction Journey::proportionTravellingTime(metresPerSecond) const
{
    return 0;
}

// TODO: Stub definition needs implementing
metresPerSecond Journey::averageTravellingSpeed(metresPerSecond) const
{
    return 0;
}

// TODO: Stub definition needs implementing
seconds Journey::durationBeforeTravellingBegins(metresPerSecond) const
{
    return 0;
}


}
