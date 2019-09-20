#pragma once

#include "material.hpp"
#include "rand_engine.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace plemma::glancy {

class Metal : public Material
{
  public:
    Metal(Vec3 const& alb, RealNum f) : albedo_(alb), fuzz_(std::min(f, Real(1))) {}

    bool Scatter(Ray const& ray_in,
                 HitRecord const& rec,
                 Vec3& attenuation,
                 Ray& scattered_ray) const override
    {
        Vec3 const reflected = Reflect(UnitVector(ray_in.Direction()), rec.normal);
        scattered_ray = Ray(rec.p, reflected + fuzz_ * GetRandomPointInUnitBall(), ray_in.Time());
        attenuation = albedo_;
        return (Dot(scattered_ray.Direction(), rec.normal) > Real(0));
    }

    // https://en.wikipedia.org/wiki/Albedo
  private:
    Vec3 albedo_;
    RealNum fuzz_;
};

}  // namespace plemma::glancy
