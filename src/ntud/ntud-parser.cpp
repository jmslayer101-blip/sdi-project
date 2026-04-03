#include <stdexcept>
#include <cmath>

#include "geometry.h"
#include "ntud/ntud-parser.h"

namespace GPS
{

namespace NTUD
{
  using namespace std;

  bool hasValidStructure(string s)
  {
      char c;
      unsigned int i;
      if (s.empty()) return false;
      if (s.size() < 5) return false;

      c = s[1];
      if (((c < 'A') || (c > 'Z')) && ((c < 'a') || (c > 'z'))) return false;
      c = s[2];
      if (((c < 'A') || (c > 'Z')) && ((c < 'a') || (c > 'z'))) return false;
      c = s[3];
      if (((c < 'A') || (c > 'Z')) && ((c < 'a') || (c > 'z'))) return false;
      c = s[4];
      if (((c < 'A') || (c > 'Z')) && ((c < 'a') || (c > 'z'))) return false;
      if (s.size() < 6) return false;
      if (s[5] != '[') return false;

      for (i = 6; true; ++i)
      {
          if (i == s.size()) return false;
          if (s[i] == '#') return false;
          if (s[i] == ';') return false;
          if (s[i] == '[') return false;
          if (s[i] == ']') break;
      }

      if (s.size() < i+4) return false;
      c = s[i+1];
      if ((c < '0') || (c > '9')) return false;
      c = s[i+2];
      if ((c < '0') || (c > '9')) return false;
      c = s[i+3];
      if ((c < '0') || (c > '9')) return false;

      if (s[i+4] != ';') return false;
      if (s.size() != i+5) return false;

      return true;
  }

  NTUD::LogEntry parseLogEntry(std::string s)
  {
      NTUD::LogEntry le;
      return le;
  }

  bool hasSufficientFields(LogEntry le)
  {
      string s = le.format;
      unsigned int n = le.fields.size();

      if (s.size() != 4)
      {
          return false;
      }

      // Convert to upper case.
      // *** A2 FIX: removed stray semicolon and added ++i ***
      for (unsigned int i = 0; i < 4; ++i)
      {
          if (s[i] >= 'a' && s[i] <= 'z')
          {
              s[i] = s[i] - 32;
          }
      }

      if (s == "NEIL" && n > 2)
      {
          return true;
      }
      else if (s == "DAVE" && n > 3)
      {
          return true;
      }
      else if (s == "ISMA" && n > 4)
      {
          return true;
      }
      else
      {
          // *** A2 FIX: corrected invalid token 'returnfalse' ***
          return false;
      }
  }

  Waypoint interpretLogEntry(NTUD::LogEntry le)
  {
      using namespace std;
      string latText, lonText, altText, latBearingText, lonBearingText;
      double lat, lon, alt;

      if (le.format == "NEIL")
      {
          latText = le.fields[0];
          lonText = le.fields[1];
          altText = le.fields[2];
          try
          {
              lat = stod(latText);
              lon = stod(lonText);
              alt = stod(altText);
              return Waypoint(lat,lon,alt);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed NEIL data field: ") + e.what());
          }
      }
      else if (le.format == "DAVE")
      {
          latText = le.fields[2];
          lonText = le.fields[3];
          altText = le.fields[1];
          try
          {
              lat = stod(latText);
              lon = stod(lonText);
              alt = stod(altText);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed DAVE data field: ") + e.what());
          }
          try
          {
              return Waypoint(lat,lon,alt);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed DAVE data field: ") + e.what());
          }
      }
      else // ISMA
      {
          latText = le.fields[0];
          latBearingText = le.fields[1];
          lonText = le.fields[2];
          lonBearingText = le.fields[3];
          altText = le.fields[4];
          try
          {
              std::string currentlyParsing = "degrees";
              string degText = "";
              string minText = "";
              string secText = "";
              for (unsigned int i = 0; i < latText.size(); ++i)
              {
                  char c = latText[i];
                  if (currentlyParsing == "degrees")
                  {
                      if (c == 'o')
                      {
                          currentlyParsing = "minutes";
                      }
                      else
                      {
                          degText += c;
                      }
                  }
                  else if (currentlyParsing == "minutes")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "seconds";
                      }
                      else
                      {
                          minText += c;
                      }
                  }
                  else if (currentlyParsing == "seconds")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "secondsSymbol";
                      }
                      else
                      {
                          secText += c;
                      }
                  }
                  else if (currentlyParsing == "secondsSymbol")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "done";
                      }
                      else
                      {
                          throw std::domain_error("malformed seconds symbol in data field: " + latText);
                      }
                  }
                  else if (currentlyParsing == "done")
                  {
                      throw std::domain_error("extra characters after seconds symbol in data field: " + latText);
                  }
              }
              if (currentlyParsing != "done")
              {
                  throw std::domain_error("missing symbols in DMS data field: " + latText);
              }
              int degs = stoi(degText);
              int mins = stoi(minText);
              int secs = stoi(secText);
              lat = fromDMS(degs,mins,secs);
              if (lat < 0)
              {
                  throw std::invalid_argument("latitude values in DMS format must be positive.  Positive/negative direction is denoted by a separate bearing indicator.");
              }

              currentlyParsing = "degrees";
              degText = "";
              minText = "";
              secText = "";
              for (unsigned int i = 0; i < lonText.size(); ++i)
              {
                  char c = lonText[i];
                  if (currentlyParsing == "degrees")
                  {
                      if (c == 'o')
                      {
                          currentlyParsing = "minutes";
                      }
                      else
                      {
                          degText += c;
                      }
                  }
                  else if (currentlyParsing == "minutes")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "seconds";
                      }
                      else
                      {
                          minText += c;
                      }
                  }
                  else if (currentlyParsing == "seconds")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "secondsSymbol";
                      }
                      else
                      {
                          secText += c;
                      }
                  }
                  else if (currentlyParsing == "secondsSymbol")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "done";
                      }
                      else
                      {
                          throw std::domain_error("malformed seconds symbol in data field: " + lonText);
                      }
                  }
                  else if (currentlyParsing == "done")
                  {
                      throw std::domain_error("extra characters after seconds symbol in data field: " + lonText);
                  }
              }
              if (currentlyParsing != "done")
              {
                  throw std::domain_error("missing symbols in DMS data field: " + lonText);
              }
              degs = stoi(degText);
              mins = stoi(minText);
              secs = stoi(secText);
              lon = fromDMS(degs,mins,secs);
              if (lon < 0)
              {
                  throw std::invalid_argument("longitude values in DMS format must be positive.  Positive/negative direction is denoted by a separate bearing indicator.");
              }

              alt = stod(altText);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed ISMA data field: ") + e.what());
          }

          if (latBearingText != "N" && latBearingText != "S")
          {
              throw domain_error("Ill-formed ISMA data field: " + latBearingText + " is an invalid bearing indicator.");
          }
          else
          {
              if (latBearingText == "S") lat = -lat;
          }
          if (lonBearingText != "E" && lonBearingText != "W")
          {
              throw domain_error("Ill-formed ISMA data field: " + lonBearingText + " is an invalid bearing indicator.");
          }
          else
          {
              if (lonBearingText == "W") lon = -lon;
          }

          try
          {
                return Waypoint(lat,lon,alt);
          }
          catch (const invalid_argument& e)
          {
                throw domain_error(string("Ill-formed ISMA data field: ") + e.what());
          }
      }
  }

}

}
