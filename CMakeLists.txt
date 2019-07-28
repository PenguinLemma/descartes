cmake_minimum_required(VERSION 3.11)

project(df_common LANGUAGES CXX)

set(cmake_utils_dir "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
list(INSERT CMAKE_MODULE_PATH 0 "${cmake_utils_dir}")

include(libraryAdditionLemma)
include(glancyCompilerOptions)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(glancy)
add_subdirectory(hitables)
add_subdirectory(materials)
add_subdirectory(math)
add_subdirectory(renderer)
add_subdirectory(scenes)
add_subdirectory(testing_utilities)
add_subdirectory(thirdparty)
add_subdirectory(utilities)
