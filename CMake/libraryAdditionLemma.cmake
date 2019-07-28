#[==[.md
# `cmakeAddingLemma`

This module includes functions to add libraries

NOTE: required CMake version need to be > 3.5, so that
we don't need to include any module as command
`cmake_parse_arguments` is already built in

## Usage

### Adding a header-only library

```
pl_add_header_only_library(
    <lib-name>
    NAMESPACE
        <namespace-ended-with::>
    SOURCES
        <file-1>
        ...
        <file-n>
    LINKED_LIBS
        <lib-1>
        ...
        <lib-m>
    COMPILER_FEATURES
        <feat-1>
        ...
        <feat-k>
)
```

### Adding a static library
```
pl_add_static_library(
    <lib-name>
    NAMESPACE
        <namespace-ended-with::>
    SOURCES
        <file-1>
        ...
        <file-n>
    PUBLIC_LINKED_LIBS
        <public-lib-1>
        ...
        <public-lib-m>
    PRIVATE_LINKED_LIBS
        <private-lib-1>
        ...
        <private-lib-k>
    COMPILER_FEATURES
        <comp-feature-1>
        ...
        <comp-feature-r>
    PUBLIC_COMPILER_OPT
        <public-comp-option-1>
        ...
        <public-comp-option-s>
    PRIVATE_COMPILER_OPT
        <private-comp-option-1>
        ...
        <private-comp-option-q>
)
```

#]==]

function (pl_add_header_only_library name)
    # Note: required CMake version is 3.10 > 3.5, so
    # we don't need to include any module as command
    # `cmake_parse_arguments` is already built in
    cmake_parse_arguments(_pl_header_only
        ""
        "NAMESPACE"
        "SOURCES;LINKED_LIBS;COMPILER_FEATURES"
        ${ARGN}
    )
    add_library(${name} INTERFACE)

    target_sources(${name}
        INTERFACE ${_pl_header_only_SOURCES}
    )

    target_include_directories(${name}
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )

    target_link_libraries(${name}
        INTERFACE ${_pl_header_only_LINKED_LIBS}
    )

    target_compile_features(${name}
        INTERFACE ${_pl_header_only_COMPILER_FEAT}
    )

    add_library(${_pl_header_only_NAMESPACE}${name}
        ALIAS ${name}
    )
endfunction()


function (pl_add_static_library name)
    cmake_parse_arguments(_pl_static_lib
        ""
        "NAMESPACE"
        "SOURCES;PUBLIC_LINKED_LIBS;PRIVATE_LINKED_LIBS;COMPILER_FEAT;PUBLIC_COMPILER_OPT;PRIVATE_COMPILER_OPT"
        ${ARGN}
    )
    add_library(${name} STATIC ${_pl_static_lib_SOURCES})

    target_include_directories(${name}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )

    target_link_libraries(${name}
        PUBLIC ${_pl_static_lib_PUBLIC_LINKED_LIBS}
        PRIVATE ${_pl_static_lib_PRIVATE_LINKED_LIBS}
    )

    target_compile_features(${name}
        PUBLIC ${_pl_static_lib_COMPILER_FEAT}
    )

    target_compile_options(${name}
        PUBLIC ${_pl_static_lib_PUBLIC_COMPILER_OPT}
        PRIVATE ${_pl_static_lib_PRIVATE_COMPILER_OPT}
    )

    add_library(${_pl_static_lib_NAMESPACE}${name}
        ALIAS ${name}
    )
endfunction()