#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <sstream>

#include "waypoint.h"
#include "ntud/ntud-logentry.h"
#include "ntud/ntud-parser.h"

using namespace GPS;
using namespace NTUD;

BOOST_AUTO_TEST_SUITE( ParseAndInterpretLogTests )

const double percentageAccuracy = 0.0001;

const std::string validLogEntryA = "@DAVID<2000-01-11T01:10:05Z,56.89,-17.5,+51.4>013;";
const std::string validLogEntryB = "@ISMAHANE<78o36'45'',N,23o42'56'',W,23.62>061;";

const GPS::Waypoint waypointA = GPS::Waypoint(-17.5,51.4,56.89);
const GPS::Waypoint waypointB = GPS::Waypoint(78.6125,-23.715556,23.62);

BOOST_AUTO_TEST_CASE( EmptyLog )
{
    std::stringstream theLog("");
    const unsigned int theExpectedSize = 0;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog);
    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( OneEntry )
{
    std::stringstream theLog;
    theLog << validLogEntryA;
    const unsigned int theExpectedSize = 1;
    const Waypoint expectedWaypoint = waypointA;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog);

    BOOST_REQUIRE_EQUAL( actualWaypoints.size() , theExpectedSize );
    const Waypoint actualWaypoint = actualWaypoints.front();
    BOOST_CHECK_CLOSE( actualWaypoint.latitude(), expectedWaypoint.latitude(), percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude(), expectedWaypoint.longitude(), percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude(), expectedWaypoint.altitude(), percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( MultipleEntries )
{
    std::stringstream theLog;
    theLog << validLogEntryA << std::endl << validLogEntryB;
    const std::vector<Waypoint> expectedWaypoints = { waypointA, waypointB };

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog);

    BOOST_REQUIRE_EQUAL( actualWaypoints.size() , expectedWaypoints.size() );
    for (unsigned int i = 0; i < expectedWaypoints.size(); ++i)
    {
        BOOST_CHECK_CLOSE( actualWaypoints[i].latitude(), expectedWaypoints[i].latitude(), percentageAccuracy );
        BOOST_CHECK_CLOSE( actualWaypoints[i].longitude(), expectedWaypoints[i].longitude(), percentageAccuracy );
        BOOST_CHECK_CLOSE( actualWaypoints[i].altitude(), expectedWaypoints[i].altitude(), percentageAccuracy );
    }
}

BOOST_AUTO_TEST_CASE( IllFormedLogEntries )
{
    std::stringstream theLog;
    theLog << validLogEntryA << std::endl
           <<  "@2345;"      << std::endl
           << validLogEntryB << std::endl
           << "@NUNO<23,A>;" << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( ValidChecksumsAreRequired )
{
    const std::string invalidChecksumLogEntryA = "@DAVID<2000-01-11T01:10:05Z,56.89,-17.5,+51.4>028;"; // should be 13
    const std::string invalidChecksumLogEntryB = "@ISMAHANE<78o36'45'',N,23o42'56'',W,23.62>052;"; // should be 61
    std::stringstream theLog;
    theLog << validLogEntryA << std::endl
           << invalidChecksumLogEntryA << std::endl
           << validLogEntryB << std::endl << invalidChecksumLogEntryB << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( InvalidStructureButValidChecksum )
{
    const std::string illformedLogEntry = "<A>065;";
    std::stringstream theLog;
    theLog << validLogEntryA << std::endl
           << illformedLogEntry << std::endl
           << validLogEntryB << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( UnrecognisedFormatCode )
{
    const std::string logEntryWithUnrecognisedFormatCode = "@PETE<NNW,40mph,14:21:58>045;";
    std::stringstream theLog;
    theLog << validLogEntryA << std::endl
           << logEntryWithUnrecognisedFormatCode << std::endl
           << validLogEntryB << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( MissingFields )
{
    const std::string logEntryWithMissingFields = "@ISMAHANE<78o36'45'',23o42'56'',23.62>036;";

    std::stringstream theLog;
    theLog << validLogEntryA << std::endl
           << logEntryWithMissingFields << std::endl
           << validLogEntryB << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( InvalidField )
{
    const std::string logEntryWithInvalidField = "@ISMAHANE<7836'45'',N,23o42'56'',W,23.62>082;"; // missing o

    std::stringstream theLog;
    theLog << validLogEntryA << std::endl
           << logEntryWithInvalidField << std::endl
           << validLogEntryB << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_SUITE_END()
