#include <boost/test/unit_test.hpp>

#include <string>

#include "ntud/ntud-logentry.h"
#include "ntud/ntud-parser.h"

using namespace GPS;
using namespace NTUD;

BOOST_AUTO_TEST_SUITE( HasValidStructureTests )

BOOST_AUTO_TEST_CASE( TypicalWellFormedLogEntries )
{
    BOOST_CHECK( hasValidStructure("#NUNO[12,17,587]001;") );
    BOOST_CHECK( hasValidStructure("#PETE[56.89,-17.5,+51.4]123;") );
    BOOST_CHECK( hasValidStructure("#ANDY[2000-01-11T01:10:05Z,56.89,-17.5o38'46'',N,+51.4o13',W]052;") );
}

BOOST_AUTO_TEST_CASE( LowerCaseFormatCodeShouldBeRejected )
{
    BOOST_CHECK( ! hasValidStructure("#pete[56.89,-17.5,+51.4]123;") );
}

BOOST_AUTO_TEST_CASE( IncorrectStartSymbol )
{
    BOOST_CHECK( ! hasValidStructure("!NUNO[12,17,587]001;") );
}

BOOST_AUTO_TEST_CASE( MissingStartSymbol )
{
    BOOST_CHECK( ! hasValidStructure("NUNO[12,17,587]001;") );
}

BOOST_AUTO_TEST_CASE( IncorrectEndSymbol )
{
    BOOST_CHECK( ! hasValidStructure("#NUNO[12,17,587]001.") );
}

BOOST_AUTO_TEST_CASE( MissingEndSymbol )
{
    BOOST_CHECK( ! hasValidStructure("#NUNO[12,17,587]001") );
}

BOOST_AUTO_TEST_CASE( MissingFormatCode )
{
    BOOST_CHECK( ! hasValidStructure("#[12,17,587]001;") );
}

BOOST_AUTO_TEST_CASE( MissingChecksum )
{
    BOOST_CHECK( ! hasValidStructure("#NUNO[12,17,587];") );
}

BOOST_AUTO_TEST_CASE( InvalidChecksumLength )
{
    BOOST_CHECK( ! hasValidStructure("#NUNO[12,17,587]01;") );
    BOOST_CHECK( ! hasValidStructure("#NUNO[12,17,587]1;") );
    BOOST_CHECK( ! hasValidStructure("#NUNO[12,17,587]0123;") );
}

BOOST_AUTO_TEST_CASE( InvalidChecksumCharacters )
{
    BOOST_CHECK( ! hasValidStructure("#NUNO[12,17,587]1A3;") );
    BOOST_CHECK( ! hasValidStructure("#NUNO[12,17,587]-24;") );
    BOOST_CHECK( ! hasValidStructure("#NUNO[12,17,587]0 1;") );
}

BOOST_AUTO_TEST_CASE( ZeroFieldsIsValid )
{
    BOOST_CHECK( hasValidStructure("#XXXX[]001;") );
}

BOOST_AUTO_TEST_CASE( SingleFieldIsValid )
{
    BOOST_CHECK( hasValidStructure("#XXXX[12]001;") );
}

BOOST_AUTO_TEST_CASE( LargeNumberOfFieldsIsValid )
{
    const std::string commas(1000,','); // 1000 fields
    BOOST_CHECK( hasValidStructure("#XXXX[" + commas + "]098;") );
}

BOOST_AUTO_TEST_CASE( MissingSuffix )
{
    BOOST_CHECK( ! hasValidStructure("") );
    BOOST_CHECK( ! hasValidStructure("#") );
    BOOST_CHECK( ! hasValidStructure("#XXXX") );
    BOOST_CHECK( ! hasValidStructure("#XXXX[") );
    BOOST_CHECK( ! hasValidStructure("#XXXX[23") );
    BOOST_CHECK( ! hasValidStructure("#XXXX[23,") );
    BOOST_CHECK( ! hasValidStructure("#XXXX[23,A") );
}

BOOST_AUTO_TEST_CASE( IllformedFormatCode )
{
    BOOST_CHECK( ! hasValidStructure("#1234[77]100;") );
    BOOST_CHECK( ! hasValidStructure("#$%&-[77]100;") );
    BOOST_CHECK( ! hasValidStructure("#A*C,[77]100;") );
}

BOOST_AUTO_TEST_CASE( InvalidReservedCharInField )
{
    BOOST_CHECK( ! hasValidStructure("#XXXX[23,[,A]100;") );
    BOOST_CHECK( ! hasValidStructure("#XXXX[23,;,A]100;") );
    BOOST_CHECK( ! hasValidStructure("#XXXX[23,#,A]100;") );
}

BOOST_AUTO_TEST_SUITE_END()
