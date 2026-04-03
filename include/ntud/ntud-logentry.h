#ifndef GPS_NTUD_LOGENTRY_H
#define GPS_NTUD_LOGENTRY_H

#include <string>
#include <vector>

namespace GPS::NTUD
{

// Stores the format code and fields of a NTUD data entry.
struct LogEntry
{
    std::string format;

    std::vector<std::string> fields;
};

}

#endif
