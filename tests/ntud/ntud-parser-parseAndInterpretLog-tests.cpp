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

const std::string validLogEntryA = "@DAVID<2000-01-11T01:10:05Z,56.89,-17.5,+51.4>0D;";
const std::string validLogEntryB = "@ISMAHANE<78d36m45s,N,23d42m56s,W,23.62>3d;";

const GPS::Waypoint waypointA = GPS::Waypoint(-17.5,51.4,56.89);
const GPS::Waypoint waypointB = GPS::Waypoint(78.6125,-23.715556,23.62);

BOOST_AUTO_TEST_CASE( EmptyLog )
{
    std::stringstream theLog("");
    std::stringstream theOutputMessages;
    const unsigned int theExpectedSize = 0;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog, theOutputMessages);
    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( OneEntry )
{
    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA;
    const unsigned int theExpectedSize = 1;
    const Waypoint expectedWaypoint = waypointA;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog, theOutputMessages);

    BOOST_REQUIRE_EQUAL( actualWaypoints.size() , theExpectedSize );
    const Waypoint actualWaypoint = actualWaypoints.front();
    BOOST_CHECK_CLOSE( actualWaypoint.latitude(), expectedWaypoint.latitude(), percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude(), expectedWaypoint.longitude(), percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude(), expectedWaypoint.altitude(), percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( MultipleEntries )
{
    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl << validLogEntryB;
    const std::vector<Waypoint> expectedWaypoints = { waypointA, waypointB };

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog, theOutputMessages);

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
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl
           <<  "@2345;"      << std::endl
           << validLogEntryB << std::endl
           << "@NUNO<23,A>;" << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog, theOutputMessages);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( ValidChecksumsAreRequired )
{
    const std::string invalidChecksumLogEntryA = "@DAVID<2000-01-11T01:10:05Z,56.89,-17.5,+51.4>1B;"; // should be 0D
    const std::string invalidChecksumLogEntryB = "@ISMAHANE<78d36m45s,N,23d42m56s,W,23.62>34;"; // should be 3d
    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl
           << invalidChecksumLogEntryA << std::endl
           << validLogEntryB << std::endl << invalidChecksumLogEntryB << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog, theOutputMessages);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( InvalidStructureButValidChecksum )
{
    const std::string illformedLogEntry = "<A>41;";
    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl
           << illformedLogEntry << std::endl
           << validLogEntryB << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog, theOutputMessages);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( UnrecognisedFormatCode )
{
    const std::string logEntryWithUnrecognisedFormatCode = "@PETE<NNW,40mph,14:21:58>2d;";
    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl
           << logEntryWithUnrecognisedFormatCode << std::endl
           << validLogEntryB << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog, theOutputMessages);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( MissingFields )
{
    const std::string logEntryWithMissingFields = "@ISMAHANE<78d36m45s,23d42m56s,23.62>24;";

    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl
           << logEntryWithMissingFields << std::endl
           << validLogEntryB << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog, theOutputMessages);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( InvalidField )
{
    const std::string logEntryWithInvalidField = "@ISMAHANE<7836m45s,N,23d42m56s,W,23.62>59;"; // missing d

    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl
           << logEntryWithInvalidField << std::endl
           << validLogEntryB << std::endl;
    const unsigned int theExpectedSize = 2;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog, theOutputMessages);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( WhitespaceOptional )
{
    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << std::endl << "    " << validLogEntryA << " \t " << std::endl << validLogEntryB << validLogEntryB << std::endl;
    const unsigned int theExpectedSize = 3;

    std::vector<Waypoint> actualWaypoints = parseAndInterpretLog(theLog, theOutputMessages);

    BOOST_CHECK_EQUAL( actualWaypoints.size() , theExpectedSize );
}

BOOST_AUTO_TEST_CASE( NoMessagesForValidEntries )
{
    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl << validLogEntryB;

    parseAndInterpretLog(theLog, theOutputMessages);

    BOOST_CHECK_EQUAL( theOutputMessages.str(), "" );
}

BOOST_AUTO_TEST_CASE( MessageForSkippedEntriesWithInvalidStructure )
{
    std::string invalidLogEntryA = "@2345;"; // invalid structure

    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl
           << invalidLogEntryA << std::endl
           << validLogEntryB << std::endl;

    parseAndInterpretLog(theLog, theOutputMessages);

    const std::string expectedMessages = "Log entry has invalid structure: " + invalidLogEntryA + "\n";
    BOOST_CHECK_EQUAL( theOutputMessages.str(), expectedMessages );
}

BOOST_AUTO_TEST_CASE( MessagesForSkippedEntriesWithInvalidChecksum )
{
    std::string invalidChecksumLogEntryA = "@DAVID<2000-01-11T01:10:05Z,56.89,-17.5,+51.4>1B;"; // should be 0D
    std::string invalidChecksumLogEntryB = "@ISMAHANE<78d36m45s,N,23d42m56s,W,23.62>34;"; // should be 3d

    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl
           << invalidChecksumLogEntryA << std::endl
           << validLogEntryB << std::endl
           << invalidChecksumLogEntryB << std::endl;

    parseAndInterpretLog(theLog, theOutputMessages);

    const std::string expectedMessages = "Log entry has invalid checksum: " + invalidChecksumLogEntryA + "\n"
                                       + "Log entry has invalid checksum: " + invalidChecksumLogEntryB + "\n";
    BOOST_CHECK_EQUAL( theOutputMessages.str(), expectedMessages );
}

BOOST_AUTO_TEST_CASE( MessageForSkippedEntrywithIncorrectNumberOfFields )
{
    std::string invalidLogEntry = "@DAVID<A>41;"; // checksum is valid, but the number of fields is incorrect

    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl
           << invalidLogEntry << std::endl
           << validLogEntryB << std::endl;

    parseAndInterpretLog(theLog, theOutputMessages);

    const std::string expectedMessages = "Log entry has incorrect number of fields: " + invalidLogEntry + "\n";

    BOOST_CHECK_EQUAL( theOutputMessages.str(), expectedMessages );
}

BOOST_AUTO_TEST_CASE( MessageForSkippedEntryWithInvalidData )
{
    std::string invalidLogEntry = "@ISMAHANE<7836m45s,N,23d42m56s,W,23.62>59;"; // missing d in degrees

    std::stringstream theLog;
    std::stringstream theOutputMessages;
    theLog << validLogEntryA << std::endl
           << invalidLogEntry << std::endl
           << validLogEntryB << std::endl;

    parseAndInterpretLog(theLog, theOutputMessages);

    const std::string expectedMessages = "Invalid data in log entry: " + invalidLogEntry + "\n";

    BOOST_CHECK_EQUAL( theOutputMessages.str(), expectedMessages );
}


BOOST_AUTO_TEST_SUITE_END()
