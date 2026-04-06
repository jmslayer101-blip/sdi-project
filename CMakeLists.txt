cmake_minimum_required(VERSION 3.5)

project(GPS2026 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_executable(journey-tests
    src/earth.cpp
    src/geometry.cpp
    src/journey.cpp
    src/waypoint.cpp

    tests/journey/BoostUTF-main.cpp
    tests/journey/journey-travellingTime-tests.cpp
  )

target_include_directories(journey-tests PRIVATE
                             ${CMAKE_SOURCE_DIR}/include
                          )

target_link_libraries(journey-tests PRIVATE
    Boost::unit_test_framework
)
