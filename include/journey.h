#ifndef GPS_JOURNEY_H
#define GPS_JOURNEY_H

#include <string>
#include <vector>

#include "types.h"
#include "timedwaypoint.h"

namespace GPS
{
  class Journey
  {
    protected:
      const std::vector<TimedWaypoint> points;

    public:
      Journey(std::vector<TimedWaypoint>);



      /* The number of waypoints in the journey.
       */
      unsigned int numberOfWaypoints() const;



      /* Total elapsed time between start and finish of the journey.
       * Throws a std::domain_error if the journey contains zero waypoints.
       * Throws a std::domain_error if the time elapsed between the first and last time stamps in the journey is negative.
       */
      seconds totalTime() const;



      /* The increase in height from the starting waypoint to the finishing waypoint.
       * Returns zero if the height difference is negative.
       * Throws a std::domain_error if the journey contains fewer than two waypoints.
       */
      metres netHeightGain() const;



      /* The sum of all the positive height differences between successive waypoints.
       * That is, downhill changes are ignored.
       * Throws a std::domain_error if the journey contains fewer than two waypoints.
       */
      metres totalHeightGain() const;



      /* The distance between the first and last waypoints of the journey.
       * This includes both vertical and horizontal distance.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      metres netLength() const;



      /* The total length of the journey. This is the sum of the distances between
       * successive waypoints, including both vertical and horizontal distance.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      metres totalLength() const;



      /* The mean speed over the course of the whole journey, taking into account both vertical and
       * horizontal distance travelled.
       * Throws a std::domain_error if the journey contains only one, or zero, waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       */
      metresPerSecond averageSpeed() const;



      /* The fastest speed between successive waypoints, taking into account both vertical and
       * horizontal distance travelled.
       * Throws a std::domain_error if the journey contains fewer than two waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       */
      metresPerSecond maxSpeed() const;



      /* The waypoint in the journey with the highest altitude.
       * If two points are equally highest, then the one that comes later is returned.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      Waypoint highestWaypoint() const;



      /* The waypoint in the journey with the lowest altitude.
       * If two points are equally lowest, then the one that comes later is returned.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      Waypoint lowestWaypoint() const;



      /* The point in the journey that is farthest north.
       * If two points are equally farthest North, then the one that comes later is returned.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      Waypoint mostNorthelyWaypoint() const;



      /* The point in the journey that is farthest south.
       * If two points are equally farthest South, then the one that comes later is returned.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      Waypoint mostSoutherlyWaypoint() const;



      /* The point in the journey that is farthest east.
       * If two points are equally farthest East, then the one that comes later is returned.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      Waypoint mostEasterlyWaypoint() const;



      /* The point in the journey that is farthest west.
       * If two points are equally farthest West, then the one that comes later is returned.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      Waypoint mostWesterlyWaypoint() const;



      /* The point in the journey that is nearest to the equator.
       * If two points are equally closest to the equator, then the one that comes later is returned.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      Waypoint mostEquatorialWaypoint() const;



      /* The point in the journey that is farthest from the equator.
       * If two points are equally far from the equator, then the one that comes later is returned.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      Waypoint leastEquatorialWaypoint() const;



      /* The fastest rate of positive altitude change between successive waypoints.
       * Returns zero if there are no positive altitude changes.
       * Throws a std::domain_error if the journey contains fewer than two waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       */
      metresPerSecond maxRateOfAscent() const;



      /* The fastest rate of negative altitude change between successive waypoints (returned as a
       * positive number).
       * Returns zero if there are no negative altitude changes.
       * Throws a std::domain_error if the journey contains fewer than two waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       */
      metresPerSecond maxRateOfDescent() const;



      /* The steepest upwards gradient (in degrees) between successive waypoints.
       * If the entire journey is downwards, then this is the least steep downwards gradient (i.e. negative).
       * If the distance (including horizontal and vertical distance) between two adjacent points does not exceed the
       * specified distance threshold, then the gradient between those two points is excluded from the calculation.
       * Throws a std::domain_error exception if no gradients are included in the calculation.
       * Throws a std::invalid_argument exception if the provided distance threshold is negative.
       */
      degrees maxGradient(metres distanceThreshold) const;



      /* The steepest downwards (i.e. negative) gradient (in degrees) between successive waypoints.
       * If the entire journey is upwards, then this is the least steep upwards gradient (i.e. positive).
       * If the distance (including horizontal and vertical distance) between two adjacent points does not exceed the
       * specified distance threshold, then the gradient between those two points is excluded from the calculation.
       * Throws a std::domain_error exception if no gradients are included in the calculation.
       * Throws a std::invalid_argument exception if the provided distance threshold is negative.
       */
      degrees minGradient(metres distanceThreshold) const;



      /* The steepest gradient (in degrees) between successive points in the journey,
       * whether upwards or downwards (i.e. either positive or negative).
       * If there are equal steepest positive and negative gradients, then the one that comes first in the journey is returned.
       * If the distance (including horizontal and vertical distance) between two adjacent points does not exceed the
       * specified distance threshold, then the gradient between those two points is excluded from the calculation.
       * Throws a std::domain_error exception if no gradients are included in the calculation.
       * Throws a std::invalid_argument exception if the provided distance threshold is negative.
       */
      degrees steepestGradient(metres distanceThreshold) const;



      /* The waypoint in the journey that is nearest to the specified waypoint.
       * If multiple points are equi-distant, the one that comes later in the journey is returned.
       * This takes into account both vertical and horizontal distance.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      Waypoint nearestWaypointTo(Waypoint targetWaypoint) const;



      /* The waypoint in the journey that is farthest from the specified waypoint.
       * If multiple points are equi-distant, the one that comes later in the journey is returned.
       * This takes into account both vertical and horizontal distance.
       * Throws a std::domain_error if the journey contains zero waypoints.
       */
      Waypoint farthestWaypointFrom(Waypoint targetWaypoint) const;



      /* The proportion of waypoints that are within the specified distance of the target waypoint.
       * This takes into account both vertical and horizontal distance.
       * Throws a std::domain_error if the journey contains zero waypoints.
       * Throws a std::invalid_argument exception if the specified distance is negative.
       */
      fraction proportionOfWaypointsNear(Waypoint targetWaypoint, metres nearDistance) const;



      /* The waypoint in the journey that comes immediately before the specified time (which may be negative).
       * Throws a std::domain_error if there are no points before that time.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       */
      Waypoint lastWaypointBefore(std::time_t targetTime) const;



      /* The waypoint in the journey that comes immediately after the specified time (which may be negative).
       * Throws a std::domain_error if there are no points after that time.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       */
      Waypoint firstWaypointAfter(std::time_t targetTime) const;



      /* The proportion of time spent during the journey where the gradient is steeper than the specified threshold.
       * This can be either uphill or downhill (i.e. positive or negative).
       * Throws a std::domain_error if the journey contains fewer than two waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided gradient threshold is negative.
       */
      fraction proportionOfTimeSteeperThan(degrees gradientThreshold) const;



      /* The proportion of time spent during the journey with an uphill gradient steeper than the specified threshold.
       * Throws a std::domain_error if the journey contains fewer than two waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided gradient threshold is negative.
       */
      fraction proportionOfTimeAscendingSteeperThan(degrees gradientThreshold) const;



      /* The proportion of time spent during the journey with an downhill gradient steeper than the specified threshold.
       * The gradient threshold argument can be provided as either a positive or negative value - both are interpreted as a downhill gradient.
       * Throws a std::domain_error if the journey contains fewer than two waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       */
      fraction proportionOfTimeDescendingSteeperThan(degrees gradientThreshold) const;




      /* Total elapsed time during the journey that is spent resting (not travelling).
       * For a period between points to count as 'resting', its speed must be less than the 'travellingSpeedThreshold' parameter,
       * taking into account both vertical and horizontal distance.
       * Throws a std::domain_error if the journey contains zero waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided speed threshold is negative.
       */
      seconds restingTime(metresPerSecond travellingSpeedThreshold) const;



      /* Total elapsed time during the journey that is spent travelling.
       * The parameter is the minimum speed required for a period between points to count as 'travelling',
       * taking into account both vertical and horizontal distance.
       * Throws a std::domain_error if the journey contains zero waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided speed threshold is negative.
       */
      seconds travellingTime(metresPerSecond travellingSpeedThreshold) const;



      /* The duration of the longest resting period spent during the journey.
       * For a period between points to count as 'resting', its speed must be less than the travellingSpeedThreshold parameter,
       * taking into account both vertical and horizontal distance. (A resting period may span several time stamps.)
       * Returns zero if the journey contains no resting periods.
       * Throws a std::domain_error if the journey contains zero waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided speed threshold is negative.
       */
      seconds longestRestingPeriod(metresPerSecond travellingSpeedThreshold) const;



      /* The duration of the longest travelling period during the journey.
       * The parameter is the minimum speed required for a period between points to count as 'travelling',
       * taking into account both vertical and horizontal distance.
       * (A travelling period may span several time stamps.)
       * Returns zero if the journey contains no travelling periods.
       * Throws a std::domain_error if the journey contains zero waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided speed threshold is negative.
       */
      seconds longestTravellingPeriod(metresPerSecond travellingSpeedThreshold) const;



      /* The mean duration of the resting periods within the journey.
       * For a period between points to count as 'resting', its speed must be less than the travellingSpeedThreshold parameter,
       * taking into account both vertical and horizontal distance.
       * (Resting through several time stamps is considered to be a single resting period.)
       * Throws a std::domain_error if the journey contains no resting periods.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided speed threshold is negative.
       */
      seconds averageRestingPeriod(metresPerSecond travellingSpeedThreshold) const;



      /* The mean duration of the travelling periods within the journey.
       * The parameter is the minimum speed required for a period between points to count as 'travelling',
       * taking into account both vertical and horizontal distance.
       * (Travelling through several time stamps is considered to be a single travelling period.)
       * Throws a std::domain_error if the journey contains no travelling periods.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided speed threshold is negative.
       */
      seconds averageTravellingPeriod(metresPerSecond travellingSpeedThreshold) const;



      /* The proportion of time during the journey that is spent resting.
       * For a period between points to count as 'resting', its speed must be less than the travellingSpeedThreshold parameter,
       * taking into account both vertical and horizontal distance.
       * Throws a std::domain_error if the journey contains fewer than two waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided speed threshold is negative.
       */
      fraction proportionRestingTime(metresPerSecond travellingSpeedThreshold) const;



      /* The proportion of time during the journey that is spent travelling.
       * The parameter is the minimum speed required for a period between points to count as 'travelling',
       * taking into account both vertical and horizontal distance.
       * Throws a std::domain_error if the journey contains fewer than two waypoints.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided speed threshold is negative.
       */
      fraction proportionTravellingTime(metresPerSecond travellingSpeedThreshold) const;



      /* The mean speed of all the time spent travelling during the journey, taking into account both
       * vertical and horizontal distance travelled.
       * The parameter is the minimum speed required for a period between points to count as 'travelling'.
       * Throws a std::domain_error if the journey does not contain any travelling periods.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided speed threshold is negative.
       */
      metresPerSecond averageTravellingSpeed(metresPerSecond travellingSpeedThreshold) const;



      /* The duration of time in the journey before travelling begins.
       * The parameter is the minimum speed required for a period between points to count as 'travelling',
       * taking into account both vertical and horizontal distance.
       * Throws a std::domain_error if the journey does not contain any travelling periods.
       * Throws a std::domain_error if the time elapsed between any two adjacent points is zero or negative.
       * Throws a std::invalid_argument exception if the provided speed threshold is negative.
       */
      seconds durationBeforeTravellingBegins(metresPerSecond travellingSpeedThreshold) const;

  };
}

#endif
