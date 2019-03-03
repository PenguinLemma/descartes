# Building Glancy with CMake

cmake_minimum_required(VERSION 3.0)

project(Glancy)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})


# c++ compiler flags
set(CMAKE_CXX_GFLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()

# Add project subdirectories
add_subdirectory(Source)
