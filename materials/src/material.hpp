#pragma once

#include "hitable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace plemma
{
namespace glancy
{

class Material
{
public:
    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec,
                         Vec3& attenuation, Ray& scattered_ray) const = 0;
};

} // namespace glancy

} // namespace plemma
