set(TEST_SOURCES
    main_test.cpp
    vec3_test.cpp
)

set(TEST_HEADERS
    vec3_test.hpp
)


set(INCLUDE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/../include
)

add_executable(GlancyTest ${TEST_SOURCES})

target_include_directories(GlancyTest PRIVATE ${INCLUDE_DIR} ${CMAKE_CURRENT_LIST_DIR}/../src)
