#include <boost/test/unit_test.hpp>

#include <string>

#include "ntud/ntud-logentry.h"
#include "ntud/ntud-parser.h"

using namespace GPS;
using namespace NTUD;

BOOST_AUTO_TEST_SUITE( HasSufficientFieldsTests )

BOOST_AUTO_TEST_CASE( SufficientFieldsDAVE )
{
    const LogEntry theLogEntry = { "DAVID", {"2000-01-11T01:10:05Z","56.89","-17.5","+51.4"} };

    BOOST_CHECK( hasCorrectNumberOfFields(theLogEntry) );
}

BOOST_AUTO_TEST_CASE( SufficientFieldsISMA )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"78o36'45''","N","23o42'56''","W","23.62"} };

    BOOST_CHECK( hasCorrectNumberOfFields(theLogEntry) );
}

BOOST_AUTO_TEST_CASE( SufficientFieldsAZI )
{
    const LogEntry theLogEntry = { "AZI", {"2001-05-15T02:08:32Z","256","78o36'45''","S","23o42'56''","E"} };

    BOOST_CHECK( hasCorrectNumberOfFields(theLogEntry) );
}

BOOST_AUTO_TEST_CASE( SufficientFieldsROB )
{
    const LogEntry theLogEntry = { "ROB", {"78o36.45","N","23o42.56","E","2001-05-15T02:08:32Z","256"} };

    BOOST_CHECK( hasCorrectNumberOfFields(theLogEntry) );
}

BOOST_AUTO_TEST_CASE( MissingFields )
{
    const LogEntry theLogEntry = { "DAVID", {"2000-01-11T01:10:05Z","56.89","+51.4"} };

    BOOST_CHECK( ! hasCorrectNumberOfFields(theLogEntry) );
}

BOOST_AUTO_TEST_CASE( ExtraFields )
{
    const LogEntry theLogEntry = { "ISMAHANE", {"78o36'45''","N","23o42'56''","E","23.62","17:43:07","14/08/74"} };

    BOOST_CHECK( ! hasCorrectNumberOfFields(theLogEntry) );
}

BOOST_AUTO_TEST_CASE( ZeroFields )
{
    const LogEntry theLogEntry = { "ISMAHANE", {} };

    BOOST_CHECK( ! hasCorrectNumberOfFields(theLogEntry) );
}

BOOST_AUTO_TEST_CASE( UnrecognisedFormatCode )
{
    const LogEntry theLogEntry = { "UNKNOWN", {"2000-01-11T01:10:05Z","78o36'45''","N","23o42'56''","E","23.62"} };

    BOOST_CHECK_THROW( hasCorrectNumberOfFields(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( DeprecatedNEILFormatNoLongerSupported )
{
    const LogEntry theLogEntry = { "NEIL", {"+45.67","-23.24","231.56"} };

    BOOST_CHECK_THROW( hasCorrectNumberOfFields(theLogEntry) , std::domain_error );
}

BOOST_AUTO_TEST_SUITE_END()
