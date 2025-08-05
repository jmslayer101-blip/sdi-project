#ifndef GPS_TIMEDWAYPOINT_H
#define GPS_TIMEDWAYPOINT_H

#include <ctime>

#include "waypoint.h"

namespace GPS
{
  struct TimedWaypoint
  {
     GPS::Waypoint waypoint;
     std::time_t timeStamp;  // time_t is usually a long int denoting the Unix Epoch (number of seconds since 1st January 1970)
  };
}

#endif
