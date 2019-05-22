set(SOURCES
    main.cpp
)

set(HEADERS
    axes_aligned_bounding_box.hpp
    bounding_volume_hierarchy.hpp
    camera.hpp
    chronometer.hpp
    constants.hpp
    dielectric.hpp
    hitable.hpp
    hitable_list.hpp
    image.hpp
    lambertian.hpp
    material.hpp
    metal.hpp
    rand_engine.hpp
    random_spheres_scene.hpp
    ray.hpp
    renderer.hpp
    scene.hpp
    sphere.hpp
    two_spheres_scene.hpp
    utilities.hpp
    vec3.hpp
)

add_executable(Glancy ${SOURCES} ${HEADERS})

set_target_properties(Glancy PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
