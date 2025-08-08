#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>

#include <boost/algorithm/string.hpp>

#include "xml-parser.h"

#include "gpx-parser.h"

namespace GPS::GPX
{
  void requireElementIs(const XML::Element& element, std::string elementName)
  {
      if (element.getName() != elementName)
      {
          throw std::domain_error("Missing '" + elementName + "' element.");
      }
  }

  void requireSubElementExists(const XML::Element& element, std::string subElementName)
  {
      if (! element.containsSubElement(subElementName) )
      {
          throw std::domain_error("Missing '" + subElementName + "' element.");
      }
  }

  void requireAttributeExists(const XML::Element& element, std::string attributeName)
  {
      if (! element.containsAttribute(attributeName))
      {
          throw std::domain_error("Missing '" + attributeName + "' attribute.");
      }
  }

  GPS::Waypoint extractWaypointFromPt(const XML::Element& ptElement)
  {
      requireAttributeExists(ptElement,"lat");
      requireAttributeExists(ptElement,"lon");
      degrees lat = std::stod(ptElement.getAttribute("lat"));
      degrees lon = std::stod(ptElement.getAttribute("lon"));

      metres ele = ptElement.containsSubElement("ele") ? std::stod(ptElement.getSubElement("ele").getLeafContent()) : 0;

      return GPS::Waypoint(lat,lon,ele);
  }

  /* Parse a date/time stamp in the ISO 8601 Interchange Standard format.
   * e.g. "2001-02-12T02:20:15Z"
   */
  std::time_t parseDateTime(const std::string& rawDateTime)
  {
      // For documentation on the format specifiers, see: https://en.cppreference.com/w/cpp/io/manip/get_time
      const std::string dateTimeFormat = "%Y-%m-%dT%H:%M:%SZ";

      tm dateTime;

      std::istringstream dateTimeStream{rawDateTime};
      dateTimeStream >> std::get_time(&dateTime, dateTimeFormat.c_str());

      if (dateTimeStream.fail())
      {
          throw std::domain_error("Malformed date/time content: " + rawDateTime);
      }

      return std::mktime(&dateTime);
  }

  std::time_t extractTimeFromPt(const XML::Element& ptElement)
  {
      if ( ptElement.containsSubElement("time") )
      {
          std::string time = ptElement.getSubElement("time").getLeafContent();
          return parseDateTime(time);
      }
      else
      {
          return 0;
      }
  }

  GPS::TimedWaypoint extractTrackPointFromTrkpt(const XML::Element& trkpt)
  {
      return { extractWaypointFromPt(trkpt),
               extractTimeFromPt(trkpt)
             };
  }

  std::vector<GPS::TimedWaypoint> extractTrackPointsFrom(const XML::Element& element)
  {
      std::vector<GPS::TimedWaypoint> trackPoints;

      for (unsigned int i = 0; i < element.countSubElements("trkpt"); ++i)
      {
          XML::Element trkpt = element.getSubElement("trkpt",i);
          trackPoints.push_back(extractTrackPointFromTrkpt(trkpt));
      }

      return trackPoints;
  }

  std::vector<GPS::TimedWaypoint> extractTrackPointsFromTrk(const XML::Element& trk)
  {
      if (trk.containsSubElement("trkseg"))
      {
          std::vector<GPS::TimedWaypoint> trackPoints;

          for (unsigned int i = 0; i < trk.countSubElements("trkseg"); ++i)
          {
              XML::Element trkseg = trk.getSubElement("trkseg",i);
              std::vector<GPS::TimedWaypoint> trackPointsInThisSegment = extractTrackPointsFrom(trkseg);
              trackPoints.insert(trackPoints.end(), trackPointsInThisSegment.begin(), trackPointsInThisSegment.end());
          }

          return trackPoints;
      }
      else
      {
          return extractTrackPointsFrom(trk);
      }
  }

  std::vector<GPS::TimedWaypoint> parseTrackStream(std::istream& gpxStream)
  {
      XML::Parser parser {gpxStream};

      XML::Element gpx = parser.parseRootElement();
      requireElementIs(gpx,"gpx");

      requireSubElementExists(gpx,"trk");
      XML::Element trk = gpx.getSubElement("trk");

      return extractTrackPointsFromTrk(trk);
  }

  std::vector<GPS::TimedWaypoint> parseTrackString(const std::string& gpxText)
  {
      std::stringstream gpxStream {gpxText};
      return parseTrackStream(gpxStream);
  }
}
