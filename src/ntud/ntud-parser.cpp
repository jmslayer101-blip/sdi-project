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
  const char startSymbol = '@';
  const char endSymbol = ';';
  const char fieldOpenBracket = '<';
  const char fieldCloseBracket = '>';
  const char fieldSeparator = ',';
  const unsigned int minFormatCodeLength = 2;
  const unsigned int checksumLength = 3;

  // Reserved characters that cannot appear within field data.
  const string reservedFieldChars = {startSymbol, endSymbol, fieldOpenBracket};

  // Named constants for NTUD format codes.
  const string formatDAVE = "DAVID";
  const string formatISMA = "ISMAHANE";
  const string formatAZI = "AZI";

  // Field count requirements for each format.
  const unsigned int daveMinFields = 4;
  const unsigned int ismaMinFields = 5;
  const unsigned int aziMinFields = 6;

  // Named constants for DMS (Degrees-Minutes-Seconds) symbols.
  const char degreesSymbol = 'o';
  const char minutesSymbol = '\'';

  // Named constants for bearing indicators.
  const string northBearing = "N";
  const string southBearing = "S";
  const string eastBearing = "E";
  const string westBearing = "W";

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

  // Field index constants for AZI format: timestamp, alt, lat, latBearing, lon, lonBearing.
  const unsigned int aziAltIndex = 1;
  const unsigned int aziLatIndex = 2;
  const unsigned int aziLatBearingIndex = 3;
  const unsigned int aziLonIndex = 4;
  const unsigned int aziLonBearingIndex = 5;

  string toUpperCase(string s)
  {
      transform(s.begin(), s.end(), s.begin(), ::toupper);
      return s;
  }

  bool isUpperAlpha(char c)
  {
      return c >= 'A' && c <= 'Z';
  }

  bool isDigit(char c)
  {
      return isdigit(static_cast<unsigned char>(c));
  }

  // Parse a DMS (Degrees-Minutes-Seconds) string like "78o36'45''" into decimal degrees.
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
          throw domain_error(string("Ill-formed ISMAHANE data field: ") + e.what());
      }

      lat = applyBearing(lat, fields[ismaLatBearingIndex], northBearing, southBearing, formatISMA);
      lon = applyBearing(lon, fields[ismaLonBearingIndex], eastBearing, westBearing, formatISMA);

      try
      {
          return Waypoint(lat, lon, alt);
      }
      catch (const invalid_argument& e)
      {
          throw domain_error(string("Ill-formed ISMAHANE data field: ") + e.what());
      }
  }

  Waypoint interpretAZI(const vector<string>& fields)
  {
      double lat, lon, alt;
      try
      {
          alt = stod(fields[aziAltIndex]);
          lat = parseDMS(fields[aziLatIndex]);
          lon = parseDMS(fields[aziLonIndex]);
      }
      catch (const invalid_argument& e)
      {
          throw domain_error(string("Ill-formed AZI data field: ") + e.what());
      }

      lat = applyBearing(lat, fields[aziLatBearingIndex], northBearing, southBearing, formatAZI);
      lon = applyBearing(lon, fields[aziLonBearingIndex], eastBearing, westBearing, formatAZI);

      try
      {
          return Waypoint(lat, lon, alt);
      }
      catch (const invalid_argument& e)
      {
          throw domain_error(string("Ill-formed AZI data field: ") + e.what());
      }
  }

  bool hasValidStructure(string s)
  {
      // Minimum length: startSymbol + 2 format chars + open + close + checksumLength + endSymbol
      const unsigned int minLength = 1 + minFormatCodeLength + 1 + 1 + checksumLength + 1;
      if (s.size() < minLength) return false;

      if (s[0] != startSymbol) return false;

      // Scan format code: 2+ uppercase alpha chars.
      unsigned int formatEnd = 1;
      while (formatEnd < s.size() && isUpperAlpha(s[formatEnd]))
      {
          ++formatEnd;
      }
      unsigned int formatLen = formatEnd - 1;
      if (formatLen < minFormatCodeLength) return false;

      if (formatEnd >= s.size() || s[formatEnd] != fieldOpenBracket) return false;

      unsigned int i = formatEnd + 1;
      for (; i < s.size(); ++i)
      {
          if (s[i] == fieldCloseBracket) break;
          if (reservedFieldChars.find(s[i]) != string::npos) return false;
      }
      if (i == s.size()) return false;

      if (s.size() < i + 1 + checksumLength + 1) return false;
      for (unsigned int j = 1; j <= checksumLength; ++j)
      {
          if (!isDigit(s[i + j])) return false;
      }

      if (s[i + checksumLength + 1] != endSymbol) return false;
      if (s.size() != i + checksumLength + 2) return false;

      return true;
  }

  unsigned int actualChecksum(string s)
  {
      size_t openBracket = s.find(fieldOpenBracket);
      size_t closeBracket = s.find(fieldCloseBracket);
      unsigned int checksum = 0;
      for (size_t i = openBracket + 1; i < closeBracket; ++i)
      {
          checksum ^= static_cast<unsigned int>(s[i]);
      }
      return checksum;
  }

  unsigned int expectedChecksum(string s)
  {
      size_t closeBracket = s.find(fieldCloseBracket);
      string checksumText = s.substr(closeBracket + 1, checksumLength);
      return stoi(checksumText);
  }

  NTUD::LogEntry parseLogEntry(std::string s)
  {
      NTUD::LogEntry le;

      // Find the open bracket to determine format code end.
      size_t openBracket = s.find(fieldOpenBracket);
      le.format = s.substr(1, openBracket - 1);
      size_t closeBracket = s.find(fieldCloseBracket);
      string fieldData = s.substr(openBracket + 1, closeBracket - openBracket - 1);

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

  bool hasCorrectNumberOfFields(LogEntry le)
  {
      string format = toUpperCase(le.format);
      unsigned int n = le.fields.size();

      if (format == formatDAVE) return n == daveMinFields;
      if (format == formatISMA) return n == ismaMinFields;
      if (format == formatAZI) return n == aziMinFields;

      throw std::domain_error("Unrecognised NTUD format code: " + format);
  }

  Waypoint interpretLogEntry(NTUD::LogEntry le)
  {
      if (le.format == formatDAVE) return interpretDAVE(le.fields);
      if (le.format == formatISMA) return interpretISMA(le.fields);
      return interpretAZI(le.fields);
  }

}

}
