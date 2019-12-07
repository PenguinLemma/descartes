#pragma once

#include "hittable.hpp"
#include "material.hpp"
#include "rand_engine.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "vec3.hpp"

#include <memory>

namespace plemma::glancy {

class Lambertian : public Material
{
  public:
    explicit Lambertian(std::shared_ptr<Texture> alb) : albedo_(alb) {}
    bool Scatter(Ray const& ray_in,
                 HitRecord const& rec,
                 Vec3& attenuation,
                 Ray& scattered_ray) const override
    {
        Vec3 direction = rec.normal + GetRandomPointInUnitBall();
        scattered_ray = Ray(rec.p, direction, ray_in.Time());
        attenuation = albedo_->GetValue(Real(0.0), Real(0.0), rec.p);
        return true;
    }

  private:
    std::shared_ptr<Texture> albedo_;
};

}  // namespace plemma::glancy
