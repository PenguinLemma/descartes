pl_add_header_only_library(
    materials
    NAMESPACE
        glancy::
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dielectric.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/lambertian.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/material.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/metal.hpp
    LINKED_LIBS
        glancy::math
        glancy::hittables
    COMPILER_FEATURES
        cxx_std_17
)

