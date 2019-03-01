#pragma once

#include "ray.h"
#include "vec3.h"
#include "hitable.h"
#include "material.h"
#include "rand_engine.h"

namespace plemma
{
namespace descartes
{

class Lambertian : public Material
{
public:
    Lambertian(const Vec3& alb) : albedo_(alb) {}
    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec,
                         Vec3& attenuation, Ray& scattered_ray) const override
    {
        Vec3 direction = rec.normal + GetRandomPointInUnitBall();
        scattered_ray = Ray(rec.p, direction);
        attenuation = albedo_;
        return true;
    }

private:
    Vec3 albedo_;
};

} // namespace descartes

} // namespace plemma
