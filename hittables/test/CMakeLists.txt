add_executable(
    hittables_test
        hittables_test.cpp
    aabb_test.cpp
)


target_link_libraries(
    hittables_test
    glancy::hittables
    glancy::testing_utilities
    Catch2::Catch
)


target_compile_features(
    hittables_test
    PUBLIC cxx_std_17
)

target_compile_options(
    hittables_test
    PRIVATE ${GLANCY_COMPILER_OPTIONS}
)
