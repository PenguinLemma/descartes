#pragma once

#include "hitable.h"
#include "ray.h"
#include "vec3.h"

namespace plemma
{
namespace descartes
{

class Material
{
public:
    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec,
                         Vec3& attenuation, Ray& scattered_ray) const = 0;
};

} // namespace descartes

} // namespace plemma
