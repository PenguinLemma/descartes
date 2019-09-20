#pragma once

#include <random>
#include "hittable.hpp"
#include "material.hpp"
#include "rand_engine.hpp"
#include "ray.hpp"
#include "utilities.hpp"
#include "vec3.hpp"

namespace plemma::glancy {

// Class implementing material of type Dielectric. Objects of
// such type of material are characterized by the following
// property with respect to interaction with light:
// - when a ray hits them, it splits into a reflected and
// a refracted ray. In this implementation, we will choose
// randomly which of the two resulting rays to follow after
// the split for each ray that hits the object.

// Remember that ri = c/v, where c is the speed of light in vacuum
// and v is the phase velocity of light in the medium (material).
// That means ri >= 1.0
template <int refractive_index_numerator, int refractive_index_denominator>
class Dielectric : public Material
{
  public:
    Dielectric()
        : refractive_index_{Real(refractive_index_numerator) / Real(refractive_index_denominator)}
    {}
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
    bool Scatter(Ray const& ray_in,
                 HitRecord const& rec,
                 Vec3& attenuation,
                 Ray& scattered_ray) const override
    {
        Vec3 outward_normal;
        Vec3 const reflected = Reflect(ray_in.Direction(), rec.normal);
        RealNum ni_over_nt;
        attenuation = Vec3(Real(1), Real(1), Real(1));
        Vec3 refracted;
        RealNum reflect_prob;
        RealNum cosine;
        if (Dot(ray_in.Direction(), rec.normal) > Real(0)) {
            outward_normal = -rec.normal;
            ni_over_nt = refractive_index_;
            cosine =
                refractive_index_ * Dot(ray_in.Direction(), rec.normal) / ray_in.Direction().Norm();
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = Real(1) / refractive_index_;
            cosine = -Dot(ray_in.Direction(), rec.normal) / ray_in.Direction().Norm();
        }

        if (Refract(ray_in.Direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = utilities::Schlick(cosine, refractive_index_);
        }
        else {
            reflect_prob = Real(1);
        }

        std::uniform_real_distribution<RealNum> distribution(Real(0), Real(1));

        if (distribution(my_engine()) < reflect_prob) {
            scattered_ray = Ray(rec.p, reflected, ray_in.Time());
        }
        else {
            scattered_ray = Ray(rec.p, refracted, ray_in.Time());
        }
        return true;
    }

  private:
    RealNum const refractive_index_;
};

// Refractive index of vacuum = 1
constexpr int kRefractiveIndexVacuumNum = 1;
constexpr int kRefractiveIndexVacuumDen = 1;
typedef Dielectric<kRefractiveIndexVacuumNum, kRefractiveIndexVacuumDen> Vacuum;

// Refractive index of ice = 1.31
constexpr int kRefractiveIndexIceNum = 131;
constexpr int kRefractiveIndexIceDen = 100;
typedef Dielectric<kRefractiveIndexIceNum, kRefractiveIndexIceDen> Ice;

// Refractive index of water = 4/3
constexpr int kRefractiveIndexWaterNum = 4;
constexpr int kRefractiveIndexWaterDen = 3;
typedef Dielectric<kRefractiveIndexWaterNum, kRefractiveIndexWaterDen> Water;

// Refractive index of olive oil = 1.47
constexpr int kRefractiveIndexOliveOilNum = 147;
constexpr int kRefractiveIndexOliveOilDen = 100;
typedef Dielectric<kRefractiveIndexOliveOilNum, kRefractiveIndexOliveOilDen> OliveOil;

// Refractive index of window glass = 1.52. Same for Crown glass (lenses)
constexpr int kRefractiveIndexWindowGlassNum = 152;
constexpr int kRefractiveIndexWindowGlassDen = 100;
typedef Dielectric<kRefractiveIndexWindowGlassNum, kRefractiveIndexWindowGlassDen> WindowGlass;

// Refractive index of flint glass = 1.62
constexpr int kRefractiveIndexFlintGlassNum = 162;
constexpr int kRefractiveIndexFlintGlassDen = 100;
typedef Dielectric<kRefractiveIndexFlintGlassNum, kRefractiveIndexFlintGlassDen> FlintGlass;

// Refractive index of sapphire = 1.77
constexpr int kRefractiveIndexSapphireNum = 177;
constexpr int kRefractiveIndexSapphireDen = 100;
typedef Dielectric<kRefractiveIndexSapphireNum, kRefractiveIndexSapphireDen> Sapphire;

// Refractive index of diamond = 2.42
constexpr int kRefractiveIndexDiamondNum = 242;
constexpr int kRefractiveIndexDiamondDen = 100;
typedef Dielectric<kRefractiveIndexDiamondNum, kRefractiveIndexDiamondDen> Diamond;

}  // namespace plemma::glancy
