#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <stdexcept>

#include "waypoint.h"
#include "ntud/ntud-logentry.h"
#include "ntud/ntud-parser.h"

using namespace GPS;
using namespace NTUD;

BOOST_AUTO_TEST_SUITE( InterpretLogEntryTests )

const double percentageAccuracy = 0.0001;
const double absoluteAccuracy = 0.0001;

BOOST_AUTO_TEST_CASE( DAVE )
{
    const LogEntry theLogEntry = { "DAVID", {"2000-01-11T01:10:05Z","56.89","-17.5","51.4"} };
    const degrees expectedLatitude = -17.5;
    const degrees expectedLongitude = 51.4;
    const degrees expectedAltitude = 56.89;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ISMA_NW )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"78d36m45s","N","23d33m56s","W","23.62"} };
    const degrees expectedLatitude = 78.6125;
    const degrees expectedLongitude = -23.565556;
    const degrees expectedAltitude = 23.62;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ISMA_SE )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03s","S","38d14m43s","E","467.21"} };
    const degrees expectedLatitude = -22.784167;
    const degrees expectedLongitude = 38.245278;
    const degrees expectedAltitude = 467.21;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( AZI )
{
    const LogEntry theLogEntry = { "AZI", {"2000-01-11T01:10:05Z","467.21","22d47m03s","S","38d14m43s","E"} };
    const degrees expectedLatitude = -22.784167;
    const degrees expectedLongitude = 38.245278;
    const degrees expectedAltitude = 467.21;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( AHMAD )
{
    const LogEntry theLogEntry = { "AHMAD", {"23.62", "78d36m45s","N","23d42m56s","W"} };
    const degrees expectedLatitude = 78.6125;
    const degrees expectedLongitude = -23.715556;
    const degrees expectedAltitude = 23.62;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ROB )
{
    const LogEntry theLogEntry = { "ROB", {"78d30m00s","N","23d06m00s","E","2001-05-15T02:08:32Z","256"} };
    const degrees expectedLatitude = 78.5;
    const degrees expectedLongitude = 23.1;
    const degrees expectedAltitude = 256;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( JO )
{
    const LogEntry theLogEntry = { "JO", {"2001-05-15T02:08:32Z","78d30m00s","N","23d06m00s","E","256","extra"} };
    const degrees expectedLatitude = 78.5;
    const degrees expectedLongitude = 23.1;
    const degrees expectedAltitude = 256;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( VISHAL )
{
    const LogEntry theLogEntry = { "VISHAL", {"23.62","23d30m00s","W","78d06m00s","N"} };
    const degrees expectedLatitude = 78.1;
    const degrees expectedLongitude = -23.5;
    const degrees expectedAltitude = 23.62;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( NegativeAltitude )
{
    const LogEntry theLogEntry = { "DAVID", {"2000-01-11T01:10:05Z","-456.8","+45.67","-23.24"} };
    const degrees expectedLatitude = 45.67;
    const degrees expectedLongitude = -23.24;
    const degrees expectedAltitude = -456.8;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( Longitude_above_90 )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"78d36m45s","N","123d33m56s","W","23.62"} };
    const degrees expectedLatitude = 78.6125;
    const degrees expectedLongitude = -123.565556;
    const degrees expectedAltitude = 23.62;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( InvalidAltitudeText )
{
    const LogEntry theLogEntry = { "DAVID", {"2000-01-11T01:10:05Z","in orbit","86.24","-23.24"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLatitudeText )
{
    const LogEntry theLogEntry = { "DAVID", {"2000-01-11T01:10:05Z","231.56","far north","-23.24"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLongitudeText )
{
    const LogEntry theLogEntry = { "DAVID", {"2000-01-11T01:10:05Z","231.56","24.78","far east"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_negative_lon )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03s","S","-38d14m26s","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingDegreesSymbol_lon )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03s","S","3814m26s","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingMinutesSymbol_lon )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03s","S","38d1426s","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingSecondsSymbol_lon )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03s","S","38d14m26","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_not_numeric_lon )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03s","S","over there","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLonBearingSymbol_lon )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03s","N","38d14m43s","Y","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_negative_lat )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"-22d47m03s","S","38d14m26s","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingDegreesSymbol_lat )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"2247m03s","S","38d14m26s","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingMinutesSymbol_lat )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d4703s","S","38d14m26s","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingSecondsSymbol_lat )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03","S","38d14m26s","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_not_numeric_lat )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"up here","S","38d14m26s","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLonBearingSymbol_lat )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03s","X","38d14m43s","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidBearingCase )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03s","n","38d14m43s","e","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidBearingWithValidPrefix )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"22d47m03s","Nowhere","38d14m43s","Wherever","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_SUITE_END()
