pl_add_header_only_library(
    textures
    NAMESPACE
        glancy::
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/checker_texture.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/constant_texture.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/texture.hpp
    LINKED_LIBS
        glancy::math
    COMPILER_FEATURES
        cxx_std_17
)

