cmake_minimum_required(VERSION 3.10)
project(Float8 VERSION 1.0
        DESCRIPTION "8-bit floating point number implementation"
        LANGUAGES C)

# Compiler options
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

# Debug build type by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/float8.c
    src/main.c
)

# Create executable
add_executable(float8 ${SOURCES})

# Installation rules
install(TARGETS float8 DESTINATION bin)
install(FILES include/float8.h DESTINATION include)
