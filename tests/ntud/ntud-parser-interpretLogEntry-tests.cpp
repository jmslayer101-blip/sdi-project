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

BOOST_AUTO_TEST_CASE( NEIL )
{
    const LogEntry theLogEntry = { "NEIL", {"+45.67","-23.24","231.56"} };
    const degrees expectedLatitude = 45.67;
    const degrees expectedLongitude = -23.24;
    const degrees expectedAltitude = 231.56;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( DAVE )
{
    const LogEntry theLogEntry = { "DAVE", {"2000-01-11T01:10:05Z","56.89","-17.5","51.4"} };
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
    const LogEntry theLogEntry = { "ISMA", {"78o36'45''","N","23o33'56''","W","23.62"} };
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
    const LogEntry theLogEntry = { "ISMA", {"22o47'03''","S","38o14'43''","E","467.21"} };
    const degrees expectedLatitude = -22.784167;
    const degrees expectedLongitude = 38.245278;
    const degrees expectedAltitude = 467.21;

    Waypoint actualWaypoint = interpretLogEntry(theLogEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( NegativeAltitude )
{
    const LogEntry theLogEntry = { "DAVE", {"2000-01-11T01:10:05Z","-456.8","+45.67","-23.24"} };
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
    const LogEntry theLogEntry = { "ISMA", {"78o36'45''","N","123o33'56''","W","23.62"} };
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
    const LogEntry theLogEntry = { "DAVE", {"2000-01-11T01:10:05Z","in orbit","86.24","-23.24"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLatitudeText )
{
    const LogEntry theLogEntry = { "DAVE", {"2000-01-11T01:10:05Z","231.56","far north","-23.24"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLongitudeText )
{
    const LogEntry theLogEntry = { "DAVE", {"2000-01-11T01:10:05Z","231.56","24.78","far east"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_negative_lon )
{
    const LogEntry theLogEntry = { "ISMA", {"22o47'03''","S","-38o14'26","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingDegreesSymbol_lon )
{
    const LogEntry theLogEntry = { "ISMA", {"22o47'03''","S","3814'26''","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingMinutesSymbol_lon )
{
    const LogEntry theLogEntry = { "ISMA", {"22o47'03''","S","38o1426''","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingSecondsSymbol_lon )
{
    const LogEntry theLogEntry = { "ISMA", {"22o47'03''","S","38o14'26","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_not_numeric_lon )
{
    const LogEntry theLogEntry = { "ISMA", {"22o47'03''","S","over there","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLonBearingSymbol_lon )
{
    const LogEntry theLogEntry = { "ISMA", {"22o47'03''","N","38o14'43''","Y","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_negative_lat )
{
    const LogEntry theLogEntry = { "ISMA", {"-22o47'03''","S","38o14'26","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingDegreesSymbol_lat )
{
    const LogEntry theLogEntry = { "ISMA", {"2247'03''","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingMinutesSymbol_lat )
{
    const LogEntry theLogEntry = { "ISMA", {"22o4703''","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingSecondsSymbol_lat )
{
    const LogEntry theLogEntry = { "ISMA", {"22o47'03","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_not_numeric_lat )
{
    const LogEntry theLogEntry = { "ISMA", {"up here","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLonBearingSymbol_lat )
{
    const LogEntry theLogEntry = { "ISMA", {"22o47'03''","X","38o14'43''","E","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidBearingCase )
{
    const LogEntry theLogEntry = { "ISMA", {"22o47'03''","n","38o14'43''","e","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidBearingWithValidPrefix )
{
    const LogEntry theLogEntry = { "ISMA", {"22o47'03''","Nowhere","38o14'43''","Wherever","467.21"} };

    BOOST_CHECK_THROW( interpretLogEntry(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_SUITE_END()
