add_executable(
    hitables_test
    hitables_test.cpp
    aabb_test.cpp
)


target_link_libraries(
    hitables_test
    glancy::hitables
    glancy::testing_utilities
    Catch2::Catch
)


target_compile_features(
    hitables_test
    PUBLIC cxx_std_17
)

target_compile_options(
    hitables_test
    PRIVATE ${GLANCY_COMPILER_OPTIONS}
)
