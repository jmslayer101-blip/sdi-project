#include <boost/test/unit_test.hpp>

#include <vector>
#include <stdexcept>
#include <ctime>

#include "journey.h"
#include "waypoint.h"
#include "timedwaypoint.h"
#include "types.h"

using namespace GPS;

/*
 * Automated test suite for Journey::travellingTime(metresPerSecond travellingSpeedThreshold).
 *
 * This function returns the total elapsed time during the journey that is spent travelling.
 * A segment counts as "travelling" if the speed between two successive waypoints meets or
 * exceeds the travellingSpeedThreshold, where speed is computed from both vertical and
 * horizontal distance.
 *
 * To simplify expected value calculations, most test cases use waypoints at the same
 * latitude (0) and longitude (0) but with different altitudes. This means horizontal
 * distance is zero, so the distance between consecutive waypoints equals the absolute
 * altitude difference. Speed is then simply: |altitude_diff| / time_diff.
 */


// Helper: creates a TimedWaypoint at lat=0, lon=0 with the given altitude and timestamp.
// Using identical lat/lon ensures horizontal distance = 0, so speed = |alt_diff| / time.
TimedWaypoint makePoint(metres altitude, std::time_t timestamp)
{
    return { Waypoint(0.0, 0.0, altitude), timestamp };
}

const double percentageTolerance = 0.001;


BOOST_AUTO_TEST_SUITE( TravellingTimeTests )


// ===== Exception test cases =====


// An empty journey (zero waypoints) should throw std::domain_error.
BOOST_AUTO_TEST_CASE( EmptyJourney_ThrowsDomainError )
{
    // Arrange
    std::vector<TimedWaypoint> noPoints = {};
    Journey journey(noPoints);
    const metresPerSecond threshold = 5.0;

    // Act & Assert
    BOOST_CHECK_THROW( journey.travellingTime(threshold), std::domain_error );
}


// A negative speed threshold should throw std::invalid_argument.
BOOST_AUTO_TEST_CASE( NegativeThreshold_ThrowsInvalidArgument )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0, 0),
        makePoint(100.0, 10)
    };
    Journey journey(points);
    const metresPerSecond negativeThreshold = -1.0;

    // Act & Assert
    BOOST_CHECK_THROW( journey.travellingTime(negativeThreshold), std::invalid_argument );
}


// Two waypoints with identical timestamps (zero elapsed time) should throw std::domain_error.
BOOST_AUTO_TEST_CASE( ZeroTimeBetweenPoints_ThrowsDomainError )
{
    // Arrange: two waypoints at the same timestamp (time = 100)
    std::vector<TimedWaypoint> points = {
        makePoint(0.0, 100),
        makePoint(50.0, 100)
    };
    Journey journey(points);
    const metresPerSecond threshold = 1.0;

    // Act & Assert
    BOOST_CHECK_THROW( journey.travellingTime(threshold), std::domain_error );
}


// Timestamps that go backwards (negative elapsed time) should throw std::domain_error.
BOOST_AUTO_TEST_CASE( NegativeTimeBetweenPoints_ThrowsDomainError )
{
    // Arrange: second timestamp (100) is earlier than first (200)
    std::vector<TimedWaypoint> points = {
        makePoint(0.0, 200),
        makePoint(50.0, 100)
    };
    Journey journey(points);
    const metresPerSecond threshold = 1.0;

    // Act & Assert
    BOOST_CHECK_THROW( journey.travellingTime(threshold), std::domain_error );
}


// Zero elapsed time in the middle of a multi-segment journey should throw std::domain_error.
BOOST_AUTO_TEST_CASE( ZeroTimeInMiddleSegment_ThrowsDomainError )
{
    // Arrange: first segment is fine (10s), second has zero time (both at t=10)
    std::vector<TimedWaypoint> points = {
        makePoint(0.0, 0),
        makePoint(100.0, 10),
        makePoint(200.0, 10)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act & Assert
    BOOST_CHECK_THROW( journey.travellingTime(threshold), std::domain_error );
}


// ===== Edge / boundary cases =====


// A single waypoint has no segments, so travelling time should be zero (not an exception).
BOOST_AUTO_TEST_CASE( SingleWaypoint_ReturnsZero )
{
    // Arrange
    std::vector<TimedWaypoint> onePoint = {
        makePoint(50.0, 0)
    };
    Journey journey(onePoint);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert
    BOOST_CHECK_EQUAL( result, 0.0 );
}


// With threshold = 0, any segment with speed >= 0 counts as travelling.
// Speed = |100 - 0| / 10 = 10 m/s >= 0, so all 10 seconds are travelling.
BOOST_AUTO_TEST_CASE( ZeroThreshold_AllMovementIsTravelling )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0, 0),
        makePoint(100.0, 10)
    };
    Journey journey(points);
    const metresPerSecond zeroThreshold = 0.0;

    // Act
    seconds result = journey.travellingTime(zeroThreshold);

    // Assert: full 10 seconds should be travelling
    BOOST_CHECK_CLOSE( result, 10.0, percentageTolerance );
}


// Boundary: speed exactly equals the threshold.
// Speed = |50 - 0| / 10 = 5.0 m/s, threshold = 5.0 m/s.
// "Minimum speed required" implies >= so this should count as travelling.
BOOST_AUTO_TEST_CASE( SpeedExactlyAtThreshold_CountsAsTravelling )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0, 0),
        makePoint(50.0, 10)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert: 50m / 10s = 5.0 m/s equals threshold, should count as travelling
    BOOST_CHECK_CLOSE( result, 10.0, percentageTolerance );
}


// Threshold higher than any segment speed: nothing counts as travelling.
// Speed = |100 - 0| / 10 = 10 m/s, threshold = 20 m/s.
BOOST_AUTO_TEST_CASE( ThresholdExceedsAllSpeeds_ReturnsZero )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0, 0),
        makePoint(100.0, 10)
    };
    Journey journey(points);
    const metresPerSecond highThreshold = 20.0;

    // Act
    seconds result = journey.travellingTime(highThreshold);

    // Assert
    BOOST_CHECK_EQUAL( result, 0.0 );
}


// ===== Typical functional cases =====


// Simple two-point journey where the segment speed exceeds the threshold.
// Speed = |100 - 0| / 10 = 10 m/s > 5 m/s threshold.
// Expected travelling time = 10 seconds.
BOOST_AUTO_TEST_CASE( TwoPoints_AboveThreshold )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0, 0),
        makePoint(100.0, 10)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert
    BOOST_CHECK_CLOSE( result, 10.0, percentageTolerance );
}


// Simple two-point journey where the segment speed is below the threshold.
// Speed = |10 - 0| / 10 = 1 m/s < 5 m/s threshold.
// Expected travelling time = 0.
BOOST_AUTO_TEST_CASE( TwoPoints_BelowThreshold_ReturnsZero )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0, 0),
        makePoint(10.0, 10)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert
    BOOST_CHECK_EQUAL( result, 0.0 );
}


// Mixed segments: some travelling, some not.
// Segment 1: |100 - 0| / (10 - 0) = 10 m/s   >= 5 -> travelling (10s)
// Segment 2: |110 - 100| / (110 - 10) = 0.1 m/s < 5 -> not travelling
// Segment 3: |210 - 110| / (120 - 110) = 10 m/s  >= 5 -> travelling (10s)
// Expected travelling time = 10 + 10 = 20 seconds.
BOOST_AUTO_TEST_CASE( MixedSegments_SomeTravellingSomeNot )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0,   0),
        makePoint(100.0, 10),
        makePoint(110.0, 110),
        makePoint(210.0, 120)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert
    BOOST_CHECK_CLOSE( result, 20.0, percentageTolerance );
}


// All segments above threshold.
// Segment 1: |100 - 0| / 10 = 10 m/s >= 5
// Segment 2: |200 - 100| / 10 = 10 m/s >= 5
// Expected travelling time = 10 + 10 = 20 seconds.
BOOST_AUTO_TEST_CASE( AllSegmentsAboveThreshold )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0,   0),
        makePoint(100.0, 10),
        makePoint(200.0, 20)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert: total time is 20s and all of it is travelling
    BOOST_CHECK_CLOSE( result, 20.0, percentageTolerance );
}


// No segments above threshold.
// Segment 1: |1 - 0| / 10 = 0.1 m/s < 5
// Segment 2: |2 - 1| / 10 = 0.1 m/s < 5
// Expected travelling time = 0.
BOOST_AUTO_TEST_CASE( NoSegmentsAboveThreshold_ReturnsZero )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0, 0),
        makePoint(1.0, 10),
        makePoint(2.0, 20)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert
    BOOST_CHECK_EQUAL( result, 0.0 );
}


// Descending altitude should still produce positive speed.
// Speed = |0 - 200| / 20 = 10 m/s >= 5 m/s threshold.
// Expected travelling time = 20 seconds.
BOOST_AUTO_TEST_CASE( DescendingAltitude_SpeedStillPositive )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(200.0, 0),
        makePoint(0.0,   20)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert: vertical distance is |200 - 0| = 200m in 20s = 10 m/s > 5
    BOOST_CHECK_CLOSE( result, 20.0, percentageTolerance );
}


// Multiple segments with varied durations, verifying correct time summation.
// Segment 1: |200 - 0| / (5 - 0) = 40 m/s     >= 5 -> travelling (5s)
// Segment 2: |201 - 200| / (105 - 5) = 0.01 m/s < 5 -> not travelling
// Segment 3: |501 - 201| / (115 - 105) = 30 m/s >= 5 -> travelling (10s)
// Segment 4: |502 - 501| / (415 - 115) = 0.003 m/s < 5 -> not travelling
// Expected travelling time = 5 + 10 = 15 seconds.
BOOST_AUTO_TEST_CASE( MultipleSegments_VariedDurations )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0,   0),
        makePoint(200.0, 5),
        makePoint(201.0, 105),
        makePoint(501.0, 115),
        makePoint(502.0, 415)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert
    BOOST_CHECK_CLOSE( result, 15.0, percentageTolerance );
}


// Stationary journey (same location for all points) with a positive threshold.
// Speed = 0 m/s for all segments, which is below any positive threshold.
// Expected travelling time = 0.
BOOST_AUTO_TEST_CASE( StationaryJourney_PositiveThreshold_ReturnsZero )
{
    // Arrange: three waypoints at the same position, 60 seconds apart each
    std::vector<TimedWaypoint> points = {
        makePoint(100.0, 0),
        makePoint(100.0, 60),
        makePoint(100.0, 120)
    };
    Journey journey(points);
    const metresPerSecond threshold = 1.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert: no movement means no travelling
    BOOST_CHECK_EQUAL( result, 0.0 );
}


// Journey that starts resting then begins travelling.
// Segment 1: |0 - 0| / 60 = 0 m/s   < 5 -> not travelling
// Segment 2: |300 - 0| / 10 = 30 m/s >= 5 -> travelling (10s)
// Expected travelling time = 10 seconds.
BOOST_AUTO_TEST_CASE( RestThenTravel )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0,   0),
        makePoint(0.0,   60),
        makePoint(300.0, 70)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert
    BOOST_CHECK_CLOSE( result, 10.0, percentageTolerance );
}


// Journey that starts travelling then stops.
// Segment 1: |300 - 0| / 10 = 30 m/s >= 5 -> travelling (10s)
// Segment 2: |300 - 300| / 60 = 0 m/s < 5 -> not travelling
// Expected travelling time = 10 seconds.
BOOST_AUTO_TEST_CASE( TravelThenRest )
{
    // Arrange
    std::vector<TimedWaypoint> points = {
        makePoint(0.0,   0),
        makePoint(300.0, 10),
        makePoint(300.0, 70)
    };
    Journey journey(points);
    const metresPerSecond threshold = 5.0;

    // Act
    seconds result = journey.travellingTime(threshold);

    // Assert
    BOOST_CHECK_CLOSE( result, 10.0, percentageTolerance );
}


BOOST_AUTO_TEST_SUITE_END()
