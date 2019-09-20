#pragma once

#include "hittable.hpp"
#include "material.hpp"
#include "rand_engine.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace plemma::glancy {

class Lambertian : public Material
{
  public:
    explicit Lambertian(Vec3 const& alb) : albedo_(alb) {}
    bool Scatter(Ray const& ray_in,
                 HitRecord const& rec,
                 Vec3& attenuation,
                 Ray& scattered_ray) const override
    {
        Vec3 direction = rec.normal + GetRandomPointInUnitBall();
        scattered_ray = Ray(rec.p, direction, ray_in.Time());
        attenuation = albedo_;
        return true;
    }

  private:
    Vec3 albedo_;
};

}  // namespace plemma::glancy
