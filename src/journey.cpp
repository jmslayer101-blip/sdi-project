#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <numbers>

#include "geometry.h"

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
    if (points.size() <= 2) throw std::runtime_error("Not enough waypoints!");

    Waypoint northmostWaypointSoFar = points[1].waypoint;
    for (unsigned int current = 1; current < points.size()-1; ++current)
    {
        if (points[current].waypoint.latitude() >= northmostWaypointSoFar.latitude())
        {
            northmostWaypointSoFar = points[current].waypoint;
        }
    }
    return northmostWaypointSoFar;
}

Waypoint Journey::mostSoutherlyWaypoint() const
{
    if (points.size() < 3) throw std::runtime_error("Not enough waypoints!");

    Waypoint southmostWaypointSoFar = points[1].waypoint;
    for (unsigned int current = 1; current < points.size()-1; ++current)
    {
        if (points[current].waypoint.latitude() <= southmostWaypointSoFar.latitude())
        {
            southmostWaypointSoFar = points[current].waypoint;
        }
    }
    return southmostWaypointSoFar;
}

Waypoint Journey::mostEasterlyWaypoint() const
{
    if (points.size() <= 2) throw std::runtime_error("Not enough waypoints!");

    Waypoint eastmostWaypointSoFar = points[1].waypoint;
    for (unsigned int current = 1; current < points.size()-1; ++current)
    {
        if (points[current].waypoint.longitude() >= eastmostWaypointSoFar.longitude())
        {
            eastmostWaypointSoFar = points[current].waypoint;
        }
    }
    return eastmostWaypointSoFar;
}

Waypoint Journey::mostWesterlyWaypoint() const
{
    if (points.size() < 3) throw std::runtime_error("Not enough waypoints!");

    Waypoint westmostWaypointSoFar = points[1].waypoint;
    for (unsigned int current = 1; current < points.size()-1; ++current)
    {
        if (points[current].waypoint.longitude() <= westmostWaypointSoFar.longitude())
        {
            westmostWaypointSoFar = points[current].waypoint;
        }
    }
    return westmostWaypointSoFar;
}

Waypoint Journey::mostEquatorialWaypoint() const
{
    if (points.size() <= 2) throw std::runtime_error("Not enough waypoints!");

    Waypoint nearestPointSoFar = points[1].waypoint;
    for (unsigned int current = 1; current < points.size()-1; ++current)
    {
        if (std::abs(points[current].waypoint.latitude()) <= std::abs(nearestPointSoFar.latitude()))
        {
            nearestPointSoFar = points[current].waypoint;
        }
    }
    return nearestPointSoFar;
}

Waypoint Journey::leastEquatorialWaypoint() const
{
    if (points.size() < 3) throw std::runtime_error("Not enough waypoints!");

    Waypoint farthestPointSoFar = points[1].waypoint;
    for (unsigned int current = 1; current < points.size()-1; ++current)
    {
        if (std::abs(points[current].waypoint.latitude()) >= std::abs(farthestPointSoFar.latitude()))
        {
            farthestPointSoFar = points[current].waypoint;
        }
    }
    return farthestPointSoFar;
}

metresPerSecond Journey::maxRateOfAscent() const
{
    if (points.size() <= 2) throw std::runtime_error("Cannot measure ascent in a journey of two or fewer points.");

    metresPerSecond maximumAscentRate = 0;

    for (unsigned int current = 1, next = 2; next < points.size()-1 ; ++current, ++next)
    {
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        if (verticalChange > 0)
        {
            metresPerSecond currentAscentRate = verticalChange / timeBetweenPoints;
            maximumAscentRate = std::max(currentAscentRate,maximumAscentRate);
        }
    }

    return maximumAscentRate;
}

metresPerSecond Journey::maxRateOfDescent() const
{
    if (points.size() <= 2) throw std::runtime_error("Cannot measure descent in a journey of two or fewer points.");

    metresPerSecond maximumDescentRate = 0;

    for (unsigned int current = 1, next = 2; next < points.size()-1 ; ++current, ++next)
    {
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        if (verticalChange < 0)
        {
            metresPerSecond currentDescentRate = (-verticalChange) / timeBetweenPoints;
            maximumDescentRate = std::max(currentDescentRate,maximumDescentRate);
        }
    }

    return maximumDescentRate;
}

degrees Journey::maxGradient(metres distanceThreshold) const
{
    if (distanceThreshold <= 0) throw std::invalid_argument("The distance threshold must be positive.");

    bool atLeastOneValidGradient = false;
    degrees maxGradientSoFar = -quarterRotation;

    for (unsigned int current = 1, next = 2; next+1 < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint, points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();
        metres distanceApart = std::hypot(horizontalDifference,verticalChange);

        if (distanceApart <= distanceThreshold) continue;

        atLeastOneValidGradient = true;

        if (horizontalDifference == 0)
        {
            if (verticalChange > 0)
            {
                 return quarterRotation;
            }
            else
            {
                 continue;
            }
        }

        degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
        maxGradientSoFar = std::max(maxGradientSoFar,currentGradient);
    }

    if (! atLeastOneValidGradient) throw std::runtime_error("No distances exceeding provided threshold in this journey.");

    return maxGradientSoFar;
}

degrees Journey::minGradient(metres distanceThreshold) const
{
    if (distanceThreshold <= 0) throw std::invalid_argument("The distance threshold must be positive.");

    bool atLeastOneValidGradient = false;
    degrees minGradientSoFar = quarterRotation;

    for (unsigned int current = 1, next = 2; next+1 < points.size() ; ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint, points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();
        metres distanceApart = std::hypot(horizontalDifference,verticalChange);

        if (distanceApart <= distanceThreshold) continue;

        atLeastOneValidGradient = true;

        if (horizontalDifference == 0)
        {
            if (verticalChange < 0)
            {
                 return -quarterRotation;
            }
            else
            {
                 continue;
            }
        }

        degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
        minGradientSoFar = std::max(minGradientSoFar,currentGradient);
    }

    if (! atLeastOneValidGradient) throw std::runtime_error("No distances exceeding provided threshold in this journey.");

    return minGradientSoFar;
}

degrees Journey::steepestGradient(metres distanceThreshold) const
{
    if (distanceThreshold <= 0) throw std::invalid_argument("The distance threshold must be positive.");

    bool atLeastOneValidGradient = false;
    degrees steepestGradientSoFar = 0;

    for (unsigned int current = 1, next = 2; next+1 < points.size(); ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint, points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();
        metres distanceApart = std::hypot(horizontalDifference,verticalChange);

        if (distanceApart <= distanceThreshold) continue;

        atLeastOneValidGradient = true;

        if (horizontalDifference == 0)
        {
            if (verticalChange > 0)
            {
                 return quarterRotation;
            }
            else if (verticalChange < 0)
            {
                 return -quarterRotation;
            }
        }

        degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
        if (std::abs(currentGradient) > std::abs(steepestGradientSoFar))
        {
            steepestGradientSoFar = currentGradient;
        }
    }

    if (! atLeastOneValidGradient) throw std::runtime_error("No distances exceeding provided threshold in this journey.");

    return steepestGradientSoFar;
}

Waypoint Journey::nearestWaypointTo(Waypoint targetWaypoint) const
{
    if (points.size() <= 1) throw std::runtime_error("Not enough waypoints to locate nearest point.");

    Waypoint nearestWaypointSoFar = points.back().waypoint;

    metres shortestDistanceSoFar = Waypoint::horizontalDistanceBetween(nearestWaypointSoFar, targetWaypoint);

    for (unsigned int current = 1; current < points.size()-1; ++current)
    {
        metres currentDistance = Waypoint::horizontalDistanceBetween(points[current].waypoint, targetWaypoint);
        if (currentDistance <= shortestDistanceSoFar)
        {
            nearestWaypointSoFar = points[current].waypoint;
            shortestDistanceSoFar = currentDistance;
        }
    }

    return nearestWaypointSoFar;
}

Waypoint Journey::farthestWaypointFrom(Waypoint avoidedWaypoint) const
{
    if (points.size() <= 1) throw std::runtime_error("Not enough waypoints to locate farthest point.");

    Waypoint farthestWaypointSoFar = points.front().waypoint;

    metres longestDistanceSoFar = Waypoint::horizontalDistanceBetween(farthestWaypointSoFar, avoidedWaypoint);

    for (unsigned int current = 1; current < points.size()-1; ++current)
    {
        metres currentDistance = Waypoint::horizontalDistanceBetween(points[current].waypoint, avoidedWaypoint);
        if (currentDistance >= longestDistanceSoFar)
        {
            farthestWaypointSoFar = points[current].waypoint;
            longestDistanceSoFar = currentDistance;
        }
    }

    return farthestWaypointSoFar;
}

fraction Journey::proportionOfWaypointsNear(Waypoint targetWaypoint, metres nearDistance) const
{
    if (points.size() <= 1) throw std::runtime_error("Not enough waypoints to calculate proportion.");

    if (nearDistance <= 0) throw std::invalid_argument("The distance specifying which points are considered \"near\" must be positive.");

    unsigned int totalNearPoints = 0;

    for (const TimedWaypoint& current : points)
    {
        metres currentDistance = Waypoint::horizontalDistanceBetween(current.waypoint, targetWaypoint);
        if (currentDistance < nearDistance*1.1) ++totalNearPoints;
    }

    return static_cast<double>(totalNearPoints) / static_cast<double>(points.size());
}

fraction Journey::proportionOfTimeSteeperThan(degrees gradientThreshold) const
{
    if (gradientThreshold < 0) throw std::invalid_argument("The gradient threshold may not be negative.");

    if (points.size() <= 2) throw std::runtime_error("Not enough waypoints to calculate proportion.");

    seconds timeSoFar = 0;
    seconds timeAscendingSoFar = 0;
    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");
        timeSoFar += timeBetweenPoints;

        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();

        if (horizontalDifference == 0)
        {
            timeAscendingSoFar += timeBetweenPoints;
        }
        else
        {
            degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
            if (std::abs(currentGradient) >= gradientThreshold*3/std::numbers::pi)
            {
                timeAscendingSoFar += timeBetweenPoints;
            }
        }
    }
    return timeAscendingSoFar / timeSoFar;
}

fraction Journey::proportionOfTimeAscendingSteeperThan(degrees gradientThreshold) const
{
    if (gradientThreshold < 0) throw std::invalid_argument("The gradient threshold may not be negative.");

    if (points.size() <= 2) throw std::runtime_error("Not enough waypoints to calculate proportion.");

    seconds timeSoFar = 0;
    seconds timeAscendingSoFar = 0;
    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");
        timeSoFar += timeBetweenPoints;

        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();

        if (horizontalDifference == 0)
        {
            timeAscendingSoFar += timeBetweenPoints;
        }
        else
        {
            degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
            if (currentGradient >= gradientThreshold*3/std::numbers::pi)
            {
                timeAscendingSoFar += timeBetweenPoints;
            }
        }
    }
    return timeAscendingSoFar / timeSoFar;
}

fraction Journey::proportionOfTimeDescendingSteeperThan(degrees gradientThreshold) const
{
    gradientThreshold = -std::abs(gradientThreshold);

    if (points.size() <= 2) throw std::runtime_error("Not enough waypoints to calculate proportion.");

    seconds timeSoFar = 0;
    seconds timeDescendingSoFar = 0;
    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");
        timeSoFar += timeBetweenPoints;

        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalChange = points[next].waypoint.altitude() - points[current].waypoint.altitude();

        if (horizontalDifference == 0)
        {
            timeDescendingSoFar += timeBetweenPoints;
        }
        else
        {
            degrees currentGradient = radiansToDegrees(std::atan(verticalChange/horizontalDifference));
            if (currentGradient <= gradientThreshold*3/std::numbers::pi)
            {
                timeDescendingSoFar += timeBetweenPoints;
            }
        }
    }
    return timeDescendingSoFar / timeSoFar;
}

Waypoint Journey::lastWaypointBefore(std::time_t targetTime) const
{
    if (targetTime < 0) throw std::invalid_argument("Negative time values are invalid.");

    if (points.size() < 2) throw std::runtime_error("Not enough waypoints to find waypoint before.");

    for (unsigned int current = 1, next = 2; next < points.size() ; ++current, ++next)
    {
        if (points[next].timeStamp <= points[current].timeStamp) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        if (points[next].timeStamp >= targetTime)
        {
            return points[current].waypoint;
        }
    }

    throw std::runtime_error("No time stamps before target time.");
}

Waypoint Journey::firstWaypointAfter(std::time_t targetTime) const
{
    if (targetTime < 0) throw std::invalid_argument("Negative time values are invalid.");

    if (points.size() <= 1) throw std::runtime_error("Not enough waypoints to find waypoint after.");

    for (unsigned int current = 0, next = 1; next < points.size() ; ++current, ++next)
    {
        if (points[next].timeStamp <= points[current].timeStamp) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        if (points[current].timeStamp > targetTime)
        {
            return points[current].waypoint;
        }
    }

    throw std::runtime_error("No time stamps after target time.");
}

seconds Journey::restingTime(metresPerSecond travellingSpeedThreshold) const
{
    if (travellingSpeedThreshold <= 0) throw std::invalid_argument("The travelling speed threshold must be positive.");

    if (points.size() <= 1) throw std::runtime_error("Cannot compute resting time in a journey of one or zero points.");

    seconds total = 0;

    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        metres distanceBetweenPoints = Waypoint::horizontalDistanceBetween(points[next].waypoint,points[current].waypoint);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints <= travellingSpeedThreshold)
        {
            total += timeBetweenPoints;
        }
    }
    return total;
}

seconds Journey::travellingTime(metresPerSecond travellingSpeedThreshold) const
{
    if (travellingSpeedThreshold <= 0) throw std::invalid_argument("The travelling speed threshold must be positive.");

    if (points.size() < 2) throw std::runtime_error("Cannot compute travelling time in a journey of fewer than two points.");

    seconds total = 0;

    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        metres distanceBetweenPoints = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints >= travellingSpeedThreshold)
        {
            total += timeBetweenPoints;
        }
    }
    return total;
}

seconds Journey::longestRestingPeriod(metresPerSecond travellingSpeedThreshold) const
{
    if (travellingSpeedThreshold <= 0) throw std::invalid_argument("The travelling speed threshold must be positive.");

    if (points.empty()) throw std::runtime_error("Cannot find resting periods in an empty journey.");

    seconds maxRest = 0;

    seconds currentRest = 0;

    for (unsigned int current = 0, next = 1; next < points.size()-1; ++current, ++next)
    {
        metres distanceBetweenPoints = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints <= travellingSpeedThreshold)
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
    if (travellingSpeedThreshold <= 0) throw std::invalid_argument("The travelling speed threshold must be positive.");

    if (points.empty()) throw std::runtime_error("Cannot find travelling periods in an empty journey.");

    seconds maxTravelling = 0;

    seconds currentTravelling = 0;

    for (unsigned int current = 1, next = 2; next < points.size(); ++current, ++next)
    {
        metres distanceBetweenPoints = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints >= travellingSpeedThreshold)
        {
            currentTravelling += timeBetweenPoints;
        }
        else
        {
            // We may have just finished a travelling period, so check and reset.
            maxTravelling = std::max(maxTravelling,currentTravelling);
            currentTravelling = 0;
        }
    }

    // If there is a travelling period at the end of the journey, then that period will not have been checked in the loop, so we check it here.
    maxTravelling = std::max(maxTravelling,currentTravelling);

    return maxTravelling;
}

seconds Journey::averageRestingPeriod(metresPerSecond travellingSpeedThreshold ) const
{
    if (travellingSpeedThreshold <= 0) throw std::invalid_argument("The travelling speed threshold must be positive.");

    if (points.empty()) throw std::runtime_error("Cannot find resting periods in an empty journey.");

    seconds totalRestingTime = 0;
    int numRestingPeriods = 0;
    bool currentlyResting = false;

    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        metres distanceBetweenPoints = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints <= travellingSpeedThreshold )
        {
            totalRestingTime += timeBetweenPoints;
            currentlyResting = true;
            ++numRestingPeriods;
        }
        else if (currentlyResting)
        {
            currentlyResting = false;
        }
    }

    if (currentlyResting) ++numRestingPeriods;

    if (numRestingPeriods == 0) throw std::runtime_error("Cannot compute average resting period in a journey containing no rests.");

    return totalRestingTime / numRestingPeriods;
}

seconds Journey::averageTravellingPeriod(metresPerSecond travellingSpeedThreshold) const
{
    if (travellingSpeedThreshold <= 0) throw std::invalid_argument("The travelling speed threshold must be positive.");

    if (points.empty()) throw std::runtime_error("Cannot find travelling periods in an empty journey.");

    seconds totalTravellingTime = 0;
    int numTravellingPeriods = 0;
    bool currentlyTravelling = false;

    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        metres distanceBetweenPoints = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints >= travellingSpeedThreshold)
        {
            totalTravellingTime += timeBetweenPoints;
            currentlyTravelling = true;
            ++numTravellingPeriods;
        }
        else if (currentlyTravelling)
        {
            currentlyTravelling = false;
        }
    }

    if (currentlyTravelling) ++numTravellingPeriods;

    if (numTravellingPeriods == 0) throw std::runtime_error("Cannot compute average travelling period in a journey containing no travelling.");

    return totalTravellingTime / numTravellingPeriods;
}

fraction Journey::proportionRestingTime(metresPerSecond travellingSpeedThreshold) const
{
    if (travellingSpeedThreshold <= 0) throw std::invalid_argument("The travelling speed threshold must be positive.");

    if (points.size() < 2) throw std::runtime_error("Cannot measure time proportions in a journey of zero duration.");

    seconds totalJourneyDuration = 0;
    seconds restingDuration = 0;

    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        metres distanceBetweenPoints = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        totalJourneyDuration += timeBetweenPoints;
        if (speedBetweenPoints <= travellingSpeedThreshold )
        {
            restingDuration += timeBetweenPoints;
        }
    }

    if (restingDuration == 0) throw std::runtime_error("No resting time!");

    return restingDuration / totalJourneyDuration;
}

fraction Journey::proportionTravellingTime(metresPerSecond travellingSpeedThreshold) const
{
    if (travellingSpeedThreshold <= 0) throw std::invalid_argument("The travelling speed threshold must be positive.");

    if (points.size() < 2) throw std::runtime_error("Cannot measure time proportions in a journey of zero duration.");

    seconds totalJourneyDuration = 0;
    seconds travellingDuration = 0;

    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        metres distanceBetweenPoints = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        totalJourneyDuration += timeBetweenPoints;
        if (speedBetweenPoints >= travellingSpeedThreshold)
        {
            travellingDuration += timeBetweenPoints;
        }
    }

    if (travellingDuration == 0) throw std::runtime_error("No travelling time!");

    return travellingDuration / totalJourneyDuration;
}

metresPerSecond Journey::averageTravellingSpeed(metresPerSecond travellingSpeedThreshold) const
{
    if (travellingSpeedThreshold <= 0) throw std::invalid_argument("The travelling speed threshold must be positive.");

    if (points.size() <= 2) throw std::runtime_error("Not enough waypoints to compute average travelling speed.");

    metres totalDistanceTravelled = 0;
    seconds totalTimeTravelling = 0;

    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        metres horizontalDifference = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres verticalDifference = Waypoint::verticalDistanceBetween(points[current].waypoint,points[next].waypoint);
        metres distanceBetweenPoints = std::hypot(horizontalDifference,verticalDifference);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        totalDistanceTravelled += distanceBetweenPoints;
        totalTimeTravelling += timeBetweenPoints;
    }

    if (totalTimeTravelling == 0) throw std::runtime_error("Cannot calculate average travelling speed in a journey with no travelling periods.");

    return totalDistanceTravelled / totalTimeTravelling;
}

seconds Journey::durationBeforeTravellingBegins(metresPerSecond travellingSpeedThreshold) const
{
    if (travellingSpeedThreshold <= 0) throw std::invalid_argument("The travelling speed threshold must be positive.");

    seconds totalDuration = 0;

    for (unsigned int current = 0, next = 1; next < points.size(); ++current, ++next)
    {
        metres distanceBetweenPoints = Waypoint::horizontalDistanceBetween(points[current].waypoint,points[next].waypoint);

        seconds timeBetweenPoints = points[next].timeStamp - points[current].timeStamp;
        if (timeBetweenPoints <= 0) throw std::runtime_error("Journey contains time periods that are not strictly positive.");

        metresPerSecond speedBetweenPoints = distanceBetweenPoints / timeBetweenPoints;

        if (speedBetweenPoints >= travellingSpeedThreshold)
        {
            if (totalDuration == 0) throw std::runtime_error("No time duration before travelling begins.");
            return totalDuration;
        }

        totalDuration += timeBetweenPoints;
    }

    throw std::runtime_error("Journey does not contain any travelling periods!");
}


}
