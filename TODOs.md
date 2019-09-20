## Functionality/optimizations
 - [x] Better memory manage (smart pointers, vector instead of list)
 - [x] Extract all random number generations to a class so that seeding and initializing is done just once
 - [x] Add motion blur
 - [x] Add Bounding Volume Hierarchies
    - [x] Add random axis sorting of boxed hittables
    - [ ] Study the possibility of a better choice of axis w.r.t. which we do the spacial ordering at each step
 - [ ] Try to switch to `constexpr` as much as possible
 - [ ] Use timing tool to find out if `constexpr` is helping
 - [ ] Add the option to check if the new `plemma::glancy::Hittable` would invade other `plemma::glancy::Hittables` in `plemma::glancy::HittableList::Add()`
 - [ ] Parallelize
 - [ ] In `axes_aligned_bounding_box.hpp`, if `Minima()` and `Maxima()` return `Vec3` instead of `Vec3 const&` the tests in `hittables_test` take much longer to finish (and will possibly be red). Study that case and see possible changes around `glancy`.

## Tools
 - [x] Add timing tool:
     - class `plemma::chronometer::SingleMeasurement`
     - free method `plemma::crhonometer::MeasureFunction()`
 - [ ] Add image visualizer (i.e., open the image on a window when done)

## Documentation
 - [ ] Add usage, license and others to `README.md`
 - [ ] Add `AnnaEstelleGlancy.md` with short explanation about her
 - [ ] Add comments per class, method and non-trivial code blocks

## Code quality
 - [x] Add `image.h`
 - [x] Rework to add scenes and renderer
 - [x] Switch to use `uint8_t`, `uin16_t`, etc
 - [ ] Improve use of namespaces
 - [ ] Check that naming conventions are preserved (as of Google's style guide or whichever is decided)
 - [x] Add automatic formatting

## Testing
Property-based testing seems a good approach, Catch2 still doesn't fully support
property-based testing, but generators might provide a good start:
https://github.com/catchorg/Catch2/blob/master/docs/generators.md
 - [x] Add [Catch2](https://github.com/catchorg/Catch2 "Catch2 repo") as testing library
 - [ ] Create tests for:
     - [x] `plemma::glancy::AxesAlignedBoundingBox`
     - [x] Free method `plemma::glancy::UnionOfAABBs`
     - [ ] All `plemma::glancy::BoundingVolumeHierarchy`'s constructors
     - [ ] `plemma::glancy::BoundingVolumeHierarchy::Hit`
     - [ ] `plemma::glancy::BoundingVolumeHierarchy::ComputeBoundingBox`
     - [ ] `plemma::glancy::Camera::GetRay`
     - [ ] `plemma::glancy::Dielectric::Scatter`
     - [ ] `plemma::glancy::HittableList::Hit` with mocked hittables derived from `plemma::glancy::Hittable`
     - [ ] `plemma::glancy::HittableList::ComputeBoundingBox` with mocked hittables derived from `plemma::glancy::Hittable`
     - [ ] `plemma::glancy::HittableList::Add`
     - [ ] `plemma::glancy::Lambertian::Scatter`
     - [ ] `plemma::glancy::Metal::Scatter`
     - [x] `plemma::glancy::Ray`
     - [ ] `plemma::glancy::Sphere::Hit`
     - [ ] `plemma::glancy::Sphere::ComputeBoundingBox` for static and dynamic sphere
     - [x] `plemma::glancy::Vec3`
