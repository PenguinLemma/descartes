## Functionality/optimizations
 - [x] Better memory manage (smart pointers, vector instead of list)
 - [x] Extract all random number generations to a class so that seeding and initializing is done just once
 - [x] Add motion blur
 - [x] Add Bounding Volume Hierarchies
    - [ ] Add random axis sorting of boxed hitables
    - [ ] Study the possibility of a better choice of axis w.r.t. which we do the spacial ordering at each step
 - [ ] Try to switch to `constexpr` as much as possible
 - [ ] Use timing tool to find out if `constexpr` is helping
 - [ ] Add the option to check if the new `plemma::glancy::Hitable` would invade other `plemma::glancy::Hitables` in `plemma::glancy::HitableList::Add()`
 - [ ] Parallelize

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
 - [ ] Add automatic formatting

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
     - [ ] `plemma::glancy::HitableList::Hit` with mocked hitables derived from `plemma::glancy::Hitable`
     - [ ] `plemma::glancy::HitableList::ComputeBoundingBox` with mocked hitables derived from `plemma::glancy::Hitable`
     - [ ] `plemma::glancy::HitableList::Add`
     - [ ] `plemma::glancy::Lambertian::Scatter`
     - [ ] `plemma::glancy::Metal::Scatter`
     - [ ] `plemma::glancy::Sphere::Hit`
     - [ ] `plemma::glancy::Sphere::ComputeBoundingBox` for static and dynamic sphere
     - [x] `plemma::glancy::Vec3`
