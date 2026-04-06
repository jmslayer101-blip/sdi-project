#include <boost/test/unit_test.hpp>

#include "ntud/ntud-logentry.h"
#include "ntud/ntud-parser.h"

using namespace GPS;
using namespace NTUD;

BOOST_AUTO_TEST_SUITE( ActualCheckSumTests )

BOOST_AUTO_TEST_CASE( SingleCharacterData )
{
    BOOST_CHECK_EQUAL( actualChecksum("@XXXX[A]123;") , 65 );
}

BOOST_AUTO_TEST_CASE( TwoCharacterData )
{
    BOOST_CHECK_EQUAL( actualChecksum("@XXXX[AB]097;") , 3 );
}

BOOST_AUTO_TEST_CASE( EmptyData )
{
    BOOST_CHECK_EQUAL( actualChecksum("@XXXX[]123;") , 0 );
}

BOOST_AUTO_TEST_CASE( TypicalData )
{
    BOOST_CHECK_EQUAL( actualChecksum("@XXXX[+45.67,-23.24,231.56]054;") , 28 );
}

BOOST_AUTO_TEST_SUITE_END()
