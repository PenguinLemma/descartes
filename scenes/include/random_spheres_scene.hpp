#pragma once

#include <memory>
#include "dielectric.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "scene.hpp"
#include "sphere.hpp"

namespace plemma {
namespace glancy {

class RandomSpheresScene : public Scene
{
  public:
    void LoadWorld() override;
    virtual const HitableList& World() const override { return world_; }
    ~RandomSpheresScene() {}

  private:
    HitableList world_;
};

inline void RandomSpheresScene::LoadWorld()
{
    std::uniform_real_distribution<RealNum> dist(Real(0), Real(1));

    // Add big sphere on top of which all other sphere will lay
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3(Real(0), Real(-1000), Real(0)),
        Real(1000),
        std::make_shared<Lambertian>(Vec3(Real(0.5), Real(0.5), Real(0.5)))));

    // Add 3 mid size spheres
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3(Real(0), Real(1), Real(0)), Real(1), std::make_shared<WindowGlass>()));

    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3(Real(-4), Real(1), Real(0)),
        Real(1),
        std::make_shared<Lambertian>(Vec3(Real(0.4), Real(0.2), Real(0.1)))));

    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3(Real(4), Real(1), Real(0)),
        Real(1),
        std::make_shared<Metal>(Vec3(Real(0.7), Real(0.6), Real(0.5)), Real(0))));

    // Add a bunch of random spheres
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            RealNum mat_choice = dist(my_engine());
            Vec3 center(Real(a) + Real(0.9) * dist(my_engine()),
                        Real(0.2),
                        Real(b) + Real(0.9) * dist(my_engine()));
            RealNum perturbance = dist(my_engine());
            auto moving_center = [=](RealNum t) {
                return center + t * Vec3(Real(0), perturbance, Real(0));
            };
            RealNum radius = Real(0.2);
            auto constant_radius = [=]([[maybe_unused]] RealNum t) { return radius; };
            bool is_static = (dist(my_engine()) > Real(0.2));

            if ((center - Vec3(Real(4), Real(0.2), Real(0))).SquaredNorm() > Real(0.9 * 0.9)) {
                if (mat_choice < Real(0.65)) {
                    Vec3 albedo(dist(my_engine()) * dist(my_engine()),
                                dist(my_engine()) * dist(my_engine()),
                                dist(my_engine()) * dist(my_engine()));
                    if (is_static) {
                        world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
                            center, radius, std::make_shared<Lambertian>(albedo)));
                    }
                    else {
                        world_.Add(std::make_shared<
                                   Sphere<decltype(moving_center), decltype(constant_radius)> >(
                            moving_center, constant_radius, std::make_shared<Lambertian>(albedo)));
                    }
                }
                else if (mat_choice < Real(0.85)) {
                    Vec3 albedo(Real(0.5) * (Real(1) + dist(my_engine())),
                                Real(0.5) * (Real(1) + dist(my_engine())),
                                Real(0.5) * (Real(1) + dist(my_engine())));
                    if (is_static) {
                        world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
                            center,
                            radius,
                            std::make_shared<Metal>(albedo, Real(0.5) * dist(my_engine()))));
                    }
                    else {
                        world_.Add(std::make_shared<
                                   Sphere<decltype(moving_center), decltype(constant_radius)> >(
                            moving_center,
                            constant_radius,
                            std::make_shared<Metal>(albedo, Real(0.5) * dist(my_engine()))));
                    }
                }
                else {
                    if (is_static) {
                        world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
                            center, radius, std::make_shared<WindowGlass>()));
                    }
                    else {
                        world_.Add(std::make_shared<
                                   Sphere<decltype(moving_center), decltype(constant_radius)> >(
                            moving_center, constant_radius, std::make_shared<WindowGlass>()));
                    }
                }
            }
        }
    }
}

}  // namespace glancy

}  // namespace plemma
