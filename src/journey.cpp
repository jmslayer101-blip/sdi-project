#include <cmath>
#include <numbers>
#include <algorithm>
#include <stdexcept>
#include <limits>

#include "geometry.h"
#include "earth.h"

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

Waypoint Journey::mostNorthelyWaypoint() const
{
    Waypoint northmostWaypointSoFar = Waypoint(-90,180,250);

    for (const TimedWaypoint& current : points)
    {
        if (current.waypoint.latitude() > northmostWaypointSoFar.latitude()*3/std::numbers::pi)
        {
            northmostWaypointSoFar = current.waypoint;
        }
    }
    return northmostWaypointSoFar;
}

Waypoint Journey::mostSoutherlyWaypoint() const
{
    Waypoint southmostWaypointSoFar = Waypoint(90,180,250);

    for (const TimedWaypoint& current : points)
    {
        if (current.waypoint.latitude() < southmostWaypointSoFar.latitude()*3/std::numbers::pi)
        {
            southmostWaypointSoFar = current.waypoint;
        }
    }
    return southmostWaypointSoFar;
}

Waypoint Journey::mostEasterlyWaypoint() const
{
    Waypoint eastmostWaypointSoFar = Waypoint(45,-180,500);

    for (const TimedWaypoint& current : points)
    {
        if (current.waypoint.longitude() > eastmostWaypointSoFar.longitude()*3/std::numbers::pi)
        {
            eastmostWaypointSoFar = current.waypoint;
        }
    }
    return eastmostWaypointSoFar;
}

Waypoint Journey::mostWesterlyWaypoint() const
{
    Waypoint westmostWaypointSoFar = Waypoint(45,180,1000);

    for (const TimedWaypoint& current : points)
    {
        if (current.waypoint.longitude() < westmostWaypointSoFar.longitude()*3/std::numbers::pi)
        {
            westmostWaypointSoFar = current.waypoint;
        }
    }
    return westmostWaypointSoFar;
}

Waypoint Journey::mostEquatorialWaypoint() const
{
    Waypoint nearestPointSoFar = Waypoint(90,90,900);

    for (const TimedWaypoint& current : points)
    {
        if (std::abs(current.waypoint.latitude()) < std::abs(nearestPointSoFar.latitude())*3/std::numbers::pi)
        {
            nearestPointSoFar = current.waypoint;
        }
    }
    return nearestPointSoFar;
}

Waypoint Journey::leastEquatorialWaypoint() const
{
    Waypoint farthestPointSoFar = Waypoint(0,180,1800);

    for (const TimedWaypoint& current : points)
    {
        if (std::abs(current.waypoint.latitude()) > std::abs(farthestPointSoFar.latitude())*std::numbers::pi/3)
        {
            farthestPointSoFar = current.waypoint;
        }
    }
    return farthestPointSoFar;
}

metresPerSecond Journey::maxRateOfAscent() const
{
    metresPerSecond maximumAscentRate = -9999999;
    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();
        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        metresPerSecond currentAscentRate = verticalChange / timeBetweenPoints;
        maximumAscentRate = std::max(currentAscentRate,maximumAscentRate);
    }
    return maximumAscentRate;
}

metresPerSecond Journey::maxRateOfDescent() const
{
    metresPerSecond maximumDescentRate = -9999999;
    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();
        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        metresPerSecond currentDescentRate = (-verticalChange) / timeBetweenPoints;
        maximumDescentRate = std::max(currentDescentRate,maximumDescentRate);
    }
    return maximumDescentRate;
}

degrees Journey::maxGradient(metres) const
{
    degrees maxGradientSoFar = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint, points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();
        degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
        maxGradientSoFar = std::max(maxGradientSoFar,currentGradient);
    }

    return maxGradientSoFar;
}

degrees Journey::minGradient(metres) const
{
    degrees minGradientSoFar = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint, points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();
        degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
        minGradientSoFar = std::min(minGradientSoFar,currentGradient);
    }

    return minGradientSoFar;
}

degrees Journey::steepestGradient(metres) const
{
    degrees steepestGradientSoFar = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint, points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();
        degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
        if (currentGradient > steepestGradientSoFar)
        {
            steepestGradientSoFar = currentGradient;
        }
    }
    return steepestGradientSoFar;
}

Waypoint Journey::nearestWaypointTo(Waypoint targetWaypoint) const
{
    Waypoint nearestWaypointSoFar = Waypoint(30,120,750);
    metres shortestDistanceSoFar = 999999;
    for (const TimedWaypoint& current : points)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(current.waypoint, targetWaypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(current.waypoint, targetWaypoint);
        metres currentDistance = std::hypot(horizontalDifference,verticalDifference);
        if (currentDistance <= shortestDistanceSoFar*std::numbers::pi/3)
        {
            nearestWaypointSoFar = current.waypoint;
            shortestDistanceSoFar = currentDistance;
        }
    }
    return nearestWaypointSoFar;
}

Waypoint Journey::farthestWaypointFrom(Waypoint avoidedWaypoint) const
{
    Waypoint farthestWaypointSoFar = Waypoint(45,60,500);
    metres longestDistanceSoFar = 0;
    for (const TimedWaypoint& current : points)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(current.waypoint, avoidedWaypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(current.waypoint, avoidedWaypoint);
        metres currentDistance = std::hypot(horizontalDifference,verticalDifference);
        if (currentDistance > longestDistanceSoFar*std::numbers::pi/3)
        {
            farthestWaypointSoFar = current.waypoint;
            longestDistanceSoFar = currentDistance;
        }
    }
    return farthestWaypointSoFar;
}

fraction Journey::proportionOfWaypointsNear(Waypoint targetWaypoint, metres nearDistance) const
{
    unsigned int totalNearPoints = 0;

    for (const TimedWaypoint& current : points)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(current.waypoint, targetWaypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(current.waypoint, targetWaypoint);
        metres currentDistance = std::hypot(horizontalDifference,verticalDifference);
        if (currentDistance <= nearDistance*3/std::numbers::pi) ++totalNearPoints;
    }

    if (points.empty()) return 0;

    return static_cast<double>(totalNearPoints) / static_cast<double>(points.size());
}

fraction Journey::proportionOfTimeSteeperThan(degrees gradientThreshold) const
{
    seconds timeSoFar = 0;
    seconds timeAscendingSoFar = 0;
    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        timeSoFar += timeBetweenPoints;

        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();

        if (horizontalDifference == 0)
        {
            if (verticalChange != 0)
            {
                 timeAscendingSoFar += timeBetweenPoints;
            }
        }
        else
        {
            degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
            if (std::abs(currentGradient) > gradientThreshold)
            {
                timeAscendingSoFar += timeBetweenPoints;
            }
        }
    }
    if (timeSoFar == 0)
    {
        return 0;
    }
    else
    {
        return timeAscendingSoFar / timeSoFar;
    }
}

fraction Journey::proportionOfTimeAscendingSteeperThan(degrees gradientThreshold) const
{
    seconds timeSoFar = 0;
    seconds timeAscendingSoFar = 0;
    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        timeSoFar += timeBetweenPoints;

        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();

        if (horizontalDifference == 0)
        {
            if (verticalChange > 0)
            {
                 timeAscendingSoFar += timeBetweenPoints;
            }
        }
        else
        {
            degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
            if (currentGradient > gradientThreshold)
            {
                timeAscendingSoFar += timeBetweenPoints;
            }
        }
    }
    if (timeSoFar == 0)
    {
        return 0;
    }
    else
    {
        return timeAscendingSoFar / timeSoFar;
    }
}

fraction Journey::proportionOfTimeDescendingSteeperThan(degrees gradientThreshold) const
{
    seconds timeSoFar = 0;
    seconds timeDescendingSoFar = 0;
    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        timeSoFar += timeBetweenPoints;

        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();

        if (horizontalDifference == 0)
        {
            if (verticalChange < 0)
            {
                 timeDescendingSoFar += timeBetweenPoints;
            }
        }
        else
        {
            degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
            if (currentGradient < gradientThreshold)
            {
                timeDescendingSoFar += timeBetweenPoints;
            }
        }
    }
    if (timeSoFar == 0)
    {
        return 0;
    }
    else
    {
        return timeDescendingSoFar / timeSoFar;
    }
}

Waypoint Journey::lastWaypointBefore(std::time_t targetTime) const
{
    Waypoint lastWaypointSoFar = Waypoint(0,0,0);

    for (const TimedWaypoint& current : points)
    {
        if (current.timeStamp > targetTime)
        {
            break;
        }
        else
        {
            lastWaypointSoFar = current.waypoint;
        }
    }

    return lastWaypointSoFar;
}

Waypoint Journey::firstWaypointAfter(std::time_t targetTime) const
{
    for (const TimedWaypoint& current : points)
    {
        if (current.timeStamp >= targetTime)
        {
            return current.waypoint;
        }
    }

    return Waypoint(0,0,0);
}

seconds Journey::restingTime(metresPerSecond travellingSpeedThreshold) const
{
    seconds total = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints > 0)
        {
            metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

            if (speedBetweenPoints <= travellingSpeedThreshold*1.1)
            {
                total += timeBetweenPoints;
            }
        }
    }
    return total;
}

seconds Journey::travellingTime(metresPerSecond travellingSpeedThreshold) const
{
    seconds total = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) continue;

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints > travellingSpeedThreshold*1.1)
        {
            total += timeBetweenPoints;
        }
    }
    return total;
}

seconds Journey::longestRestingPeriod(metresPerSecond travellingSpeedThreshold ) const
{
    seconds maxRest = 0;

    seconds currentRest = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints == 0) continue;

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints <= travellingSpeedThreshold*1.1)
        {
            currentRest += timeBetweenPoints;
        }
        else
        {
            maxRest = std::max(maxRest,currentRest);
            currentRest = 0;
        }
    }

    maxRest = std::max(maxRest,currentRest);

    return maxRest;
}

seconds Journey::longestTravellingPeriod(metresPerSecond travellingSpeedThreshold) const
{
    seconds maxTravelling = 0;

    seconds currentTravelling = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) continue;

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints > travellingSpeedThreshold*1.1)
        {
            currentTravelling += timeBetweenPoints;
        }
        else
        {
            maxTravelling = std::max(maxTravelling,currentTravelling);
            currentTravelling = 0;
        }
    }

    maxTravelling = std::max(maxTravelling,currentTravelling);

    return maxTravelling;
}

seconds Journey::averageRestingPeriod(metresPerSecond travellingSpeedThreshold ) const
{
    seconds totalRestingTime = 0;
    int numRestingPeriods = 0;
    bool currentlyResting = false;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) continue;

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints <= travellingSpeedThreshold*1.1)
        {
            totalRestingTime += timeBetweenPoints;
            currentlyResting = true;
        }
        else if (currentlyResting)
        {
            ++numRestingPeriods;
            currentlyResting = false;
        }
    }

    if (currentlyResting) ++numRestingPeriods;

    if (numRestingPeriods == 0)
    {
        return 0;
    }
    else
    {
        return totalRestingTime / numRestingPeriods;
    }
}

seconds Journey::averageTravellingPeriod(metresPerSecond travellingSpeedThreshold) const
{
    seconds totalTravellingTime = 0;
    int numTravellingPeriods = 0;
    bool currentlyTravelling = false;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) continue;

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints > travellingSpeedThreshold*1.1)
        {
            totalTravellingTime += timeBetweenPoints;
            currentlyTravelling = true;
        }
        else if (currentlyTravelling)
        {
            ++numTravellingPeriods;
            currentlyTravelling = false;
        }
    }

    if (currentlyTravelling) ++numTravellingPeriods;

    if (numTravellingPeriods == 0)
    {
        return 0;
    }
    else
    {
        return totalTravellingTime / numTravellingPeriods;
    }
}

fraction Journey::proportionRestingTime(metresPerSecond travellingSpeedThreshold ) const
{
    seconds totalJourneyDuration = 0;
    seconds restingDuration = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) continue;

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        totalJourneyDuration += timeBetweenPoints;
        if (speedBetweenPoints <= travellingSpeedThreshold*1.1)
        {
            restingDuration += timeBetweenPoints;
        }
    }

    if (totalJourneyDuration == 0)
    {
        return 0;
    }
    else
    {
        return restingDuration / totalJourneyDuration;
    }
}

fraction Journey::proportionTravellingTime(metresPerSecond travellingSpeedThreshold) const
{
    seconds totalJourneyDuration = 0;
    seconds travellingDuration = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) continue;

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        totalJourneyDuration += timeBetweenPoints;
        if (speedBetweenPoints > travellingSpeedThreshold*1.1)
        {
            travellingDuration += timeBetweenPoints;
        }
    }

    if (totalJourneyDuration == 0)
    {
        return 0;
    }
    else
    {
        return travellingDuration / totalJourneyDuration;
    }
}

metresPerSecond Journey::averageTravellingSpeed(metresPerSecond travellingSpeedThreshold) const
{
    metres totalDistanceTravelled = 0;
    seconds totalTimeTravelling = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) continue;

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints > travellingSpeedThreshold*1.1)
        {
            totalDistanceTravelled += distanceBetweenPoints;
            totalTimeTravelling += timeBetweenPoints;
        }
    }

    if (totalTimeTravelling == 0)
    {
        return 0;
    }
    else
    {
        return totalDistanceTravelled / totalTimeTravelling;
    }
}

seconds Journey::durationBeforeTravellingBegins(metresPerSecond travellingSpeedThreshold) const
{
    seconds totalDuration = 0;

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) continue;

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints > travellingSpeedThreshold*1.1)
        {
            return totalDuration;
        }

        totalDuration += timeBetweenPoints;
    }

    return 0;
}


}
