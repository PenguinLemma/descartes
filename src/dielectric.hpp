#pragma once

#include "vec3.hpp"
#include "ray.hpp"
#include "hitable.hpp"
#include "material.hpp"
#include "utilities.hpp"
#include "rand_engine.hpp"
#include <random>

namespace plemma
{
namespace glancy
{

// Class implementing material of type Dielectric. Objects of
// such type of material are characterized by the following
// property with respect to interaction with light:
// - when a ray hits them, it splits into a reflected and
// a refracted ray. In this implementation, we will choose
// randomly which of the two resulting rays to follow after
// the split for each ray that hits the object.
class Dielectric : public Material
{
public:
    Dielectric(RealNum ri) : refraction_index_(ri) {}

    // Returns false if the ray is absorbed by this material
    // and true otherwise. It also computes the scattered ray
    // resulting from the interaction between ray and material
    // and the attenuation that should be applied to the
    // computed color.
    // In the case of dielectrics:
    // - Return value is always true, rays are never absorbed.
    // - Attenuation is always (1, 1, 1) (which means "no attenuation")
    // - Scattered ray is chosen randomly between a reflected and
    //   a refracted ray. In the case the refracted ray doesn't exist,
    //   we choose the reflected one with probability 1. See method
    //   `bool Refract(const Vec3& v, const Vec3& n, RealNum ni_over_nt, Vec3& refracted)`
    //   for more information
    virtual bool Scatter(const Ray& ray_in, const HitRecord& rec,
                         Vec3& attenuation, Ray& scattered_ray) const override
    {
        Vec3 outward_normal;
        Vec3 reflected = Reflect(ray_in.Direction(), rec.normal);
        RealNum ni_over_nt;
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
            reflect_prob = utilities::Schlick(cosine, refraction_index_);
        }
        else
        {
            reflect_prob = 1.0;
        }

        std::uniform_real_distribution<RealNum> distribution(0.0, 1.0);

        if (distribution(my_engine()) < reflect_prob)
        {
            scattered_ray = Ray(rec.p, reflected, ray_in.Time());
        }
        else
        {
            scattered_ray = Ray(rec.p, refracted, ray_in.Time());
        }
        return true;
    }

private:
    RealNum refraction_index_;
};

} // namespace glancy

} // namespace plemma
