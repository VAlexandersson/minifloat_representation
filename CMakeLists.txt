cmake_minimum_required(VERSION 3.10)
project(Float8 VERSION 1.0
        DESCRIPTION "8-bit floating point number implementation"
        LANGUAGES C)

# Compiler options
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(SOURCES
    src/float8.c
    src/main.c
)

set(TEST_SOURCES
    test/test_float8.c
    test/test_main.c
)

# Main library
add_library(float8_lib ${SOURCES})
target_include_directories(float8_lib PUBLIC src)

# Test executable
add_executable(float8_test ${TEST_SOURCES})
target_link_libraries(float8_test float8_lib)
target_include_directories(float8_test PRIVATE test)

# Main executable
add_executable(float8 src/main.c)
target_link_libraries(float8 float8_lib)

install(TARGETS float8 DESTINATION bin)
