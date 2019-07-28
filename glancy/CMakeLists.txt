add_executable(glancy ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(
    glancy
    PUBLIC
        glancy::hitables
        glancy::materials
        glancy::math
        glancy::renderer
        glancy::scenes
        glancy::utilities
)

target_compile_features(
    glancy
    PUBLIC cxx_std_17
)

target_compile_options(
    glancy
    PRIVATE ${GLANCY_COMPILER_OPTIONS}
)