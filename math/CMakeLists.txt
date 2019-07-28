pl_add_header_only_library(
    math
    NAMESPACE
        glancy::
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ray.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vec3.hpp
    LINKED_LIBS
        glancy::utilities
    COMPILER_FEATURES
        cxx_std_17
)

add_subdirectory(test)
