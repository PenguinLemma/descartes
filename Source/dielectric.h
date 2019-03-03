#pragma once

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "material.h"
#include "utilities.h"
#include "rand_engine.h"
#include <random>

namespace plemma
{
namespace glancy
{

class Dielectric : public Material
{
public:
    Dielectric(RealNum ri) : refraction_index_(ri) {}
    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec,
                         Vec3& attenuation, Ray& scattered_ray) const override
    {
        Vec3 outward_normal;
        Vec3 reflected = Reflect(ray_in.Direction(), rec.normal);
        RealNum ni_over_nt; // this name has to be improved
        attenuation = Vec3(1.0, 1.0, 1.0);
        Vec3 refracted;
        RealNum reflect_prob;
        RealNum cosine;
        if (Dot(ray_in.Direction(), rec.normal) > 0.0)
        {
            outward_normal = -rec.normal;
            ni_over_nt = refraction_index_;
            cosine = refraction_index_ * Dot(ray_in.Direction(), rec.normal) / ray_in.Direction().Norm();
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / refraction_index_;
            cosine = -Dot(ray_in.Direction(), rec.normal) / ray_in.Direction().Norm();
        }

        if (Refract(ray_in.Direction(), outward_normal, ni_over_nt, refracted))
        {
            reflect_prob = Schlick(cosine, refraction_index_);
        }
        else
        {
            reflect_prob = 1.0;
        }

        std::uniform_real_distribution<RealNum> distribution(0.0, 1.0);

        if (distribution(my_engine()) < reflect_prob)
        {
            scattered_ray = Ray(rec.p, reflected);
        }
        else
        {
            scattered_ray = Ray(rec.p, refracted);
        }
        return true;
    }

private:
    RealNum refraction_index_;
};

} // namespace glancy

} // namespace plemma
