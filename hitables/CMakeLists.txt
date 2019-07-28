pl_add_header_only_library(
    hitables
    NAMESPACE
        glancy::
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/axes_aligned_bounding_box.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/bounding_volume_hierarchy.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/hitable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/hitable_list.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sphere.hpp
    LINKED_LIBS
        glancy::math
        glancy::materials

    COMPILER_FEATURES
        cxx_std_17
)

add_subdirectory(test)