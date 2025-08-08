cmake_minimum_required(VERSION 3.5)

project(GPS2026 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(journey-tests
    include/earth.h
    include/geometry.h
    include/journey.h
    include/timedwaypoint.h
    include/types.h
    include/waypoint.h
    include/gpx/gpx-parser.h
    include/xml/xml-element.h
    include/xml/xml-parser.h

    src/earth.cpp
    src/geometry.cpp
    src/journey.cpp
    src/waypoint.cpp
    src/gpx/gpx-parser.cpp
    src/xml/xml-element.cpp
    src/xml/xml-parser.cpp

    tests/journey/journey-tests.cpp
  )
target_sources(journey-tests PRIVATE ${CMAKE_SOURCE_DIR}/data/NorthYorkMoors.gpx)

target_include_directories(journey-tests PRIVATE
                             ${CMAKE_SOURCE_DIR}/include
                             ${CMAKE_SOURCE_DIR}/include/gpx/
                             ${CMAKE_SOURCE_DIR}/include/xml/
                          )
