set(TEST_SOURCES
    main_test.cpp
    vec3_test.cpp
)

set(TEST_HEADERS
    vec3_test.hpp
    container_approx.hpp
)


set(INCLUDE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/../include
)

add_executable(GlancyTest ${TEST_SOURCES} ${TEST_HEADERS})

target_include_directories(GlancyTest PRIVATE ${INCLUDE_DIR} ${CMAKE_CURRENT_LIST_DIR}/../src)

set_target_properties(GlancyTest PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
