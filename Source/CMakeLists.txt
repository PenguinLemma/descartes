set(SOURCES
    main.cpp
)

set(HEADERS
    camera.h
    chronometer.h
    constants.h
    dielectric.h
    hitable.h
    hitable_list.h
    image.h
    lambertian.h
    material.h
    metal.h
    rand_engine.h
    renderer.h
    ray.h
    scene.h
    sphere.h
    two_spheres_scene.h
    utilities.h
    vec3.h
)

add_executable(Glancy ${SOURCES} ${HEADERS})
