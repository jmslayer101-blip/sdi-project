#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <ios>
#include <ostream>
#include <sstream>

#include "ntud/ntud-logentry.h"
#include "ntud/ntud-parser.h"

using namespace GPS;
using namespace NTUD;

BOOST_AUTO_TEST_SUITE( ParseLogEntryTests )

// Utility function for comparing log entries for equality.
void checkLogEntriesEqual(const NTUD::LogEntry & actual, const NTUD::LogEntry & expected);


BOOST_AUTO_TEST_CASE( TypicalLogEntry )
{
    const std::string logEntryRawText = "@NUNO<+45.67,-23.24,231.56,40mph>123;";
    const LogEntry expectedLogEntry = { "NUNO", {"+45.67","-23.24","231.56","40mph"} };

    LogEntry actualLogEntry = parseLogEntry(logEntryRawText);

    checkLogEntriesEqual(actualLogEntry , expectedLogEntry);
}

BOOST_AUTO_TEST_CASE( EmptyLogEntry )
{
    const std::string logEntryRawText = "@XXXX<>123;";
    const LogEntry expectedLogEntry = { "XXXX", { "" } };

    LogEntry actualLogEntry = parseLogEntry(logEntryRawText);

    checkLogEntriesEqual(actualLogEntry , expectedLogEntry);
}

BOOST_AUTO_TEST_CASE( SomeEmptyFields )
{
    const std::string logEntryRawText = "@XXXX<abc,,20,>078;";
    const LogEntry expectedLogEntry = { "XXXX", { "abc", "", "20", "" } };

    LogEntry actualLogEntry = parseLogEntry(logEntryRawText);

    checkLogEntriesEqual(actualLogEntry , expectedLogEntry);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Displaying vectors directly using the built-in BOOST macros is tricky.
 * For reasons I don't fully understand, just overloading operator<< here doesn't work.
 * See: https://stackoverflow.com/questions/17572583/boost-check-fails-to-compile-operator-for-custom-types
 *
 * So instead we introduce an auxilliary function for formatting the error message for mismatched vectors.
 */

std::ostream& operator<<(std::ostream& outputStream, const std::vector<std::string> & vec)
{
    outputStream << '{';
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) outputStream << ',';
        outputStream << '"' << *it << '"';
    }
    outputStream << '}';
    return outputStream;
}

std::string formatMismatchedFormatCode(std::string actualCode, std::string expectedCode)
{
    std::stringstream outputMessage;
    outputMessage << "parseLogEntry() has failed, mismatched format code [ \"" << actualCode << "\" != \"" << expectedCode << "\" ]";
    return outputMessage.str();
}

std::string formatMismatchedFieldData(const std::vector<std::string> & actualFields,
                                      const std::vector<std::string> & expectedFields)
{
    std::stringstream outputMessage;
    outputMessage << "parseLogEntry() has failed, mismatched fields [ " << actualFields << " != " << expectedFields << " ]";
    return outputMessage.str();
}

void checkLogEntriesEqual(const NTUD::LogEntry & actualLogEntry, const NTUD::LogEntry & expectedLogEntry)
{
    BOOST_CHECK_MESSAGE( actualLogEntry.format == expectedLogEntry.format,
                         formatMismatchedFormatCode(actualLogEntry.format, expectedLogEntry.format) );

    BOOST_CHECK_MESSAGE( actualLogEntry.fields == expectedLogEntry.fields,
                         formatMismatchedFieldData(actualLogEntry.fields,expectedLogEntry.fields) );
}

BOOST_AUTO_TEST_SUITE_END()
