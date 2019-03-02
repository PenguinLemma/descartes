set(SOURCES
    main.cpp
)

set(HEADERS
    camera.h
    constants.h
    dielectric.h
    hitable.h
    hitable_list.h
    image.h
    lambertian.h
    material.h
    metal.h
    rand_engine.h
    ray.h
    sphere.h
    utilities.h
    vec3.h
)

add_executable(Descartes ${SOURCES} ${HEADERS})
