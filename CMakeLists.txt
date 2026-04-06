cmake_minimum_required(VERSION 3.20)

project(GPS2026 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_executable(ntud-parser-tests
    src/earth.cpp
    src/geometry.cpp
    src/ntud/ntud-parser.cpp
    src/waypoint.cpp

    tests/ntud/BoostUTF-main.cpp
    tests/ntud/ntud-parser-hasSufficientFields-tests.cpp
    tests/ntud/ntud-parser-hasValidStructure-tests.cpp
    tests/ntud/ntud-parser-interpretLogEntry-tests.cpp
    tests/ntud/ntud-parser-parseLogEntry-tests.cpp
)

target_include_directories(ntud-parser-tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ntud-parser-tests PRIVATE
    Boost::unit_test_framework
)
