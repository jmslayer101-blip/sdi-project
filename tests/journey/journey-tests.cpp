#include <iostream>
#include <fstream>
#include <filesystem>

#include "gpx-parser.h"
#include "journey.h"

using namespace GPS;

using std::cout;
using std::endl;

int main()
{
    std::string filepath = "../data/NorthYorkMoors.gpx";

    if (! std::filesystem::exists(filepath))
    {
        cout << "Could not open log file: " + filepath << endl;
        cout << "(If you're running at the command-line, you need to 'cd' into the 'bin/' directory first.)";
    }
    else
    {
        std::fstream gpxData {filepath};

        Journey exampleJourney = GPX::parseTrackStream(gpxData);

        cout << "Number of waypoints: " << exampleJourney.numberOfWaypoints() << endl;

        cout << "Total time: " << exampleJourney.totalTime() << "s" << endl;

        cout << "Net height gain: " << exampleJourney.netHeightGain() << "m" << endl;

        cout << "Total height gain: " << exampleJourney.totalHeightGain() << "m" << endl;

        cout << "Net length: " << exampleJourney.netLength() << "m" << endl;

        cout << "Total length: " << exampleJourney.totalLength() << "m" << endl;

        cout << "Average speed: " << exampleJourney.averageSpeed() << "m/s" << endl;

        cout << "Maximum speed: " << exampleJourney.maxSpeed() << "m/s" << endl;

        Waypoint highestPoint = exampleJourney.highestWaypoint();
        cout << "Highest waypoint: " << highestPoint.latitude() << "o lat by "
             << highestPoint.longitude() << "o lon at altitude "
             << highestPoint.altitude() << "m" << endl;

        Waypoint lowestPoint = exampleJourney.lowestWaypoint();
        cout << "Lowest waypoint: " << lowestPoint.latitude() << "o lat by "
             << lowestPoint.longitude() << "o lon at altitude "
             << lowestPoint.altitude() << "m" << endl;
    }
}
