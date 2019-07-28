add_executable(
    math_test
    math_test.cpp
    ray_test.cpp
    vec3_test.cpp
)

target_link_libraries(
    math_test
    glancy::math
    glancy::testing_utilities
    Catch2::Catch
)

target_compile_features(
    math_test
    PUBLIC cxx_std_17
)

target_compile_options(
    math_test
    PRIVATE ${GLANCY_COMPILER_OPTIONS}
)