#pragma once

#include "ray.hpp"
#include "vec3.hpp"
#include "hitable.hpp"
#include "material.hpp"
#include "rand_engine.hpp"

namespace plemma
{
namespace glancy
{

class Lambertian : public Material
{
public:
    Lambertian(const Vec3& alb) : albedo_(alb) {}
    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec,
                         Vec3& attenuation, Ray& scattered_ray) const override
    {
        Vec3 direction = rec.normal + GetRandomPointInUnitBall();
        scattered_ray = Ray(rec.p, direction, ray_in.Time());
        attenuation = albedo_;
        return true;
    }

private:
    Vec3 albedo_;
};

} // namespace glancy

} // namespace plemma
