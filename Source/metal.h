#pragma once

#include "vec3.h"
#include "ray.h"
#include "material.h"
#include "rand_engine.h"

namespace plemma
{
namespace glancy
{

class Metal : public Material
{
public:
    Metal(const Vec3& alb, const RealNum f) : albedo_(alb), fuzz_(std::min(f, static_cast<RealNum>(1.0))) {}

    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec,
                         Vec3& attenuation, Ray& scattered_ray) const override
    {
        Vec3 reflected = Reflect(UnitVector(ray_in.Direction()), rec.normal);
        scattered_ray = Ray(rec.p, reflected + fuzz_*GetRandomPointInUnitBall());
        attenuation = albedo_;
        return (Dot(scattered_ray.Direction(), rec.normal) > 0.0);
    }

    // https://en.wikipedia.org/wiki/Albedo
private:
    Vec3 albedo_;
    RealNum fuzz_;
};

} // namespace glancy

} // namespace plemma
