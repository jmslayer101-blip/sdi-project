#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <cctype>

#include "geometry.h"
#include "ntud/ntud-parser.h"

namespace GPS
{

namespace NTUD
{
  using namespace std;

  // Named constants for NTUD log entry structure.
  const char startSymbol = '#';
  const char endSymbol = ';';
  const char fieldOpenBracket = '[';
  const char fieldCloseBracket = ']';
  const char fieldSeparator = ',';
  const unsigned int formatCodeLength = 4;
  const unsigned int checksumLength = 3;

  // Reserved characters that cannot appear within field data.
  const string reservedFieldChars = {startSymbol, endSymbol, fieldOpenBracket};

  // Named constants for NTUD format codes.
  const string formatNEIL = "NEIL";
  const string formatDAVE = "DAVE";
  const string formatISMA = "ISMA";

  // Field count requirements for each format.
  const unsigned int neilMinFields = 3;
  const unsigned int daveMinFields = 4;
  const unsigned int ismaMinFields = 5;

  // Named constants for DMS (Degrees-Minutes-Seconds) symbols.
  const char degreesSymbol = 'o';
  const char minutesSymbol = '\'';

  // Named constants for bearing indicators.
  const string northBearing = "N";
  const string southBearing = "S";
  const string eastBearing = "E";
  const string westBearing = "W";

  // Field index constants for NEIL format: lat, lon, alt.
  const unsigned int neilLatIndex = 0;
  const unsigned int neilLonIndex = 1;
  const unsigned int neilAltIndex = 2;

  // Field index constants for DAVE format: timestamp, alt, lat, lon.
  const unsigned int daveAltIndex = 1;
  const unsigned int daveLatIndex = 2;
  const unsigned int daveLonIndex = 3;

  // Field index constants for ISMA format: lat, latBearing, lon, lonBearing, alt.
  const unsigned int ismaLatIndex = 0;
  const unsigned int ismaLatBearingIndex = 1;
  const unsigned int ismaLonIndex = 2;
  const unsigned int ismaLonBearingIndex = 3;
  const unsigned int ismaAltIndex = 4;

  // Case-insensitive comparison of a format string to a target (uppercase) format code.
  string toUpperCase(string s)
  {
      transform(s.begin(), s.end(), s.begin(), ::toupper);
      return s;
  }

  bool isAlpha(char c)
  {
      return isalpha(static_cast<unsigned char>(c));
  }

  bool isDigit(char c)
  {
      return isdigit(static_cast<unsigned char>(c));
  }

  // Parse a DMS (Degrees-Minutes-Seconds) string like "78o36'45''" into decimal degrees.
  // Throws std::domain_error if the format is malformed.
  degrees parseDMS(const string& dmsText)
  {
      string degText, minText, secText;
      enum class ParseState { Degrees, Minutes, Seconds, SecondsTick, Done };
      ParseState state = ParseState::Degrees;

      for (char c : dmsText)
      {
          switch (state)
          {
              case ParseState::Degrees:
                  if (c == degreesSymbol) state = ParseState::Minutes;
                  else degText += c;
                  break;
              case ParseState::Minutes:
                  if (c == minutesSymbol) state = ParseState::Seconds;
                  else minText += c;
                  break;
              case ParseState::Seconds:
                  if (c == minutesSymbol) state = ParseState::SecondsTick;
                  else secText += c;
                  break;
              case ParseState::SecondsTick:
                  if (c == minutesSymbol) state = ParseState::Done;
                  else throw domain_error("malformed seconds symbol in data field: " + dmsText);
                  break;
              case ParseState::Done:
                  throw domain_error("extra characters after seconds symbol in data field: " + dmsText);
          }
      }

      if (state != ParseState::Done)
      {
          throw domain_error("missing symbols in DMS data field: " + dmsText);
      }

      int degs = stoi(degText);
      int mins = stoi(minText);
      int secs = stoi(secText);
      degrees result = fromDMS(degs, mins, secs);

      if (result < 0)
      {
          throw invalid_argument("values in DMS format must be positive.  Positive/negative direction is denoted by a separate bearing indicator.");
      }

      return result;
  }

  // Interpret a Waypoint from NEIL format fields: lat, lon, alt as decimal strings.
  Waypoint interpretNEIL(const vector<string>& fields)
  {
      try
      {
          double lat = stod(fields[neilLatIndex]);
          double lon = stod(fields[neilLonIndex]);
          double alt = stod(fields[neilAltIndex]);
          return Waypoint(lat, lon, alt);
      }
      catch (const invalid_argument& e)
      {
          throw domain_error(string("Ill-formed NEIL data field: ") + e.what());
      }
  }

  // Interpret a Waypoint from DAVE format fields: timestamp, alt, lat, lon as decimal strings.
  Waypoint interpretDAVE(const vector<string>& fields)
  {
      double lat, lon, alt;
      try
      {
          lat = stod(fields[daveLatIndex]);
          lon = stod(fields[daveLonIndex]);
          alt = stod(fields[daveAltIndex]);
      }
      catch (const invalid_argument& e)
      {
          throw domain_error(string("Ill-formed DAVE data field: ") + e.what());
      }
      try
      {
          return Waypoint(lat, lon, alt);
      }
      catch (const invalid_argument& e)
      {
          throw domain_error(string("Ill-formed DAVE data field: ") + e.what());
      }
  }

  // Apply a bearing indicator to negate a coordinate value if necessary.
  double applyBearing(double value, const string& bearing,
                      const string& positiveBearing, const string& negativeBearing,
                      const string& formatName)
  {
      if (bearing != positiveBearing && bearing != negativeBearing)
      {
          throw domain_error("Ill-formed " + formatName + " data field: " + bearing + " is an invalid bearing indicator.");
      }
      return (bearing == negativeBearing) ? -value : value;
  }

  // Interpret a Waypoint from ISMA format fields: lat(DMS), latBearing, lon(DMS), lonBearing, alt.
  Waypoint interpretISMA(const vector<string>& fields)
  {
      double lat, lon, alt;
      try
      {
          lat = parseDMS(fields[ismaLatIndex]);
          lon = parseDMS(fields[ismaLonIndex]);
          alt = stod(fields[ismaAltIndex]);
      }
      catch (const invalid_argument& e)
      {
          throw domain_error(string("Ill-formed ISMA data field: ") + e.what());
      }

      lat = applyBearing(lat, fields[ismaLatBearingIndex], northBearing, southBearing, formatISMA);
      lon = applyBearing(lon, fields[ismaLonBearingIndex], eastBearing, westBearing, formatISMA);

      try
      {
          return Waypoint(lat, lon, alt);
      }
      catch (const invalid_argument& e)
      {
          throw domain_error(string("Ill-formed ISMA data field: ") + e.what());
      }
  }

  bool hasValidStructure(string s)
  {
      // Minimum length: # + 4 format chars + [ + ] + 3 checksum digits + ;
      const unsigned int minLength = 1 + formatCodeLength + 1 + 1 + checksumLength + 1;
      if (s.size() < minLength) return false;

      // Check start symbol.
      if (s[0] != startSymbol) return false;

      // Check format code is all alphabetic.
      for (unsigned int i = 1; i <= formatCodeLength; ++i)
      {
          if (!isAlpha(s[i])) return false;
      }

      // Check field open bracket.
      unsigned int fieldStart = 1 + formatCodeLength;
      if (s[fieldStart] != fieldOpenBracket) return false;

      // Scan field data for close bracket, rejecting reserved characters.
      unsigned int i = fieldStart + 1;
      for (; i < s.size(); ++i)
      {
          if (s[i] == fieldCloseBracket) break;
          if (reservedFieldChars.find(s[i]) != string::npos) return false;
      }
      if (i == s.size()) return false;

      // Check checksum: exactly checksumLength digits after the close bracket.
      if (s.size() < i + 1 + checksumLength + 1) return false;
      for (unsigned int j = 1; j <= checksumLength; ++j)
      {
          if (!isDigit(s[i + j])) return false;
      }

      // Check end symbol and no trailing characters.
      if (s[i + checksumLength + 1] != endSymbol) return false;
      if (s.size() != i + checksumLength + 2) return false;

      return true;
  }

  NTUD::LogEntry parseLogEntry(std::string s)
  {
      NTUD::LogEntry le;

      // Extract the format code.
      le.format = s.substr(1, formatCodeLength);

      // Find the brackets that delimit the field data.
      size_t openBracket = s.find(fieldOpenBracket);
      size_t closeBracket = s.find(fieldCloseBracket);
      string fieldData = s.substr(openBracket + 1, closeBracket - openBracket - 1);

      // Split field data by the separator.
      string currentField;
      for (char c : fieldData)
      {
          if (c == fieldSeparator)
          {
              le.fields.push_back(currentField);
              currentField.clear();
          }
          else
          {
              currentField += c;
          }
      }
      le.fields.push_back(currentField);

      return le;
  }

  bool hasSufficientFields(LogEntry le)
  {
      string format = toUpperCase(le.format);
      unsigned int n = le.fields.size();

      if (format.size() != formatCodeLength) return false;

      if (format == formatNEIL) return n > neilMinFields - 1;
      if (format == formatDAVE) return n > daveMinFields - 1;
      if (format == formatISMA) return n > ismaMinFields - 1;

      return false;
  }

  Waypoint interpretLogEntry(NTUD::LogEntry le)
  {
      if (le.format == formatNEIL) return interpretNEIL(le.fields);
      if (le.format == formatDAVE) return interpretDAVE(le.fields);
      return interpretISMA(le.fields);
  }

}

}
