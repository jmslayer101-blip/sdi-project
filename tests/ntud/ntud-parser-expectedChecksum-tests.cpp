#include <boost/test/unit_test.hpp>

#include "ntud/ntud-logentry.h"
#include "ntud/ntud-parser.h"

using namespace GPS;
using namespace NTUD;

BOOST_AUTO_TEST_SUITE( ExpectedCheckSumTests )

BOOST_AUTO_TEST_CASE( TypicalChecksum )
{
    BOOST_CHECK_EQUAL( expectedChecksum("@XXXX[-38.7]123;") , 123 );
}

BOOST_AUTO_TEST_CASE( LeadingZero )
{
    BOOST_CHECK_EQUAL( expectedChecksum("@XXXX[67,N]045;") , 45 );
}

BOOST_AUTO_TEST_CASE( EmptyFields )
{
    BOOST_CHECK_EQUAL( expectedChecksum("@XXXX[]112;") , 112 );
}

BOOST_AUTO_TEST_SUITE_END()
