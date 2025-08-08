#ifndef GPS_GPX_PARSER_H
#define GPS_GPX_PARSER_H

#include <vector>
#include <istream>

#include "timedwaypoint.h"

namespace GPS::GPX
{
  // Parse a stream of GPX data containing a track.
  std::vector<GPS::TimedWaypoint> parseTrackStream(std::istream&);

  // Parse a GPX data string containing a track.
  std::vector<GPS::TimedWaypoint> parseTrackString(const std::string&);

}

#endif
