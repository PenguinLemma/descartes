#pragma once

#include "hitable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace plemma::glancy {

class Material
{
  public:
    virtual bool Scatter(Ray const& ray_in,
                         HitRecord const& rec,
                         Vec3& attenuation,
                         Ray& scattered_ray) const = 0;
};

}  // namespace plemma::glancy
