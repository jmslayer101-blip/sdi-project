#ifndef GPS_NTUD_PARSER_H
#define GPS_NTUD_PARSER_H

#include <string>
#include <vector>
#include <istream>

#include "ntud-logentry.h"
#include "waypoint.h"

namespace GPS::NTUD
{
  // Check whether the provided string conforms to the structure of an NTUD log entry.
  // Does not check the details of any specific NTUD format.
  bool hasValidStructure(std::string);



  // Computes the checksum of the raw text between the checksum delimiters.
  // The checksum computation is the XOR reduction of the ASCII character codes of that text.
  // Pre-condition: the argument string conforms to the structure of an NTUD log entry.
  unsigned int actualChecksum(std::string);



  // Parses the raw text of a NTUD log entry and stores the data contents in a structured form in an NTUD::LogEntry.
  // Pre-condition: the argument string conforms to the structure of an NTUD log entry.
  NTUD::LogEntry parseLogEntry(std::string);



  // Checks whether the log entry contains the correct number of fields for the entry type of the format code.
  // Throws std::domain_error if the log entry has an unrecognised format code.
  bool hasCorrectNumberOfFields(NTUD::LogEntry);



  // Interprets the GPS location data in the log entry and constructs a Waypoint object.
  // Pre-condition: the log entry has the correct amount of fields.
  // Throws std::domain_error if the data fields contain invalid content.
  GPS::Waypoint interpretLogEntry(NTUD::LogEntry);

}

#endif
