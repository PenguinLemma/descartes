#pragma once

#include "scene.hpp"
#include "sphere.hpp"
#include "lambertian.hpp"
#include "dielectric.hpp"
#include "metal.hpp"
#include <memory>

namespace plemma
{
namespace glancy
{

class RandomSpheresScene : public Scene
{
public:
    void LoadWorld() override;
    virtual const HitableList& World() const override { return world_; }
    ~RandomSpheresScene() {}
private:
    HitableList world_;
};

void RandomSpheresScene::LoadWorld()
{
    std::uniform_real_distribution<RealNum> dist(0.0, 1.0);

    // Add big sphere on top of which all other sphere will lay
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(Vec3(0.0, -1000.0, 0.0), 1000.0,
                                        std::make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5))));

    // Add 3 mid size spheres
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(Vec3(0.0, 1.0, 0.0), 1.0,
                                        std::make_shared<Dielectric>(1.5)));

    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(Vec3(-4.0, 1.0, 0.0), 1.0,
                                        std::make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1))));

    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(Vec3(4.0, 1.0, 0.0), 1.0,
                                        std::make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0)));

    // Add a bunch of random spheres
    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            RealNum mat_choice = dist(my_engine());
            Vec3 center(static_cast<RealNum>(a) + 0.9 * dist(my_engine()),
                        0.2,
                        static_cast<RealNum>(b) + 0.9 * dist(my_engine()));
            RealNum perturbance = dist(my_engine());
            auto moving_center = [=](RealNum t) { return center + t * Vec3(0.0, perturbance, 0.0); };
            RealNum radius = 0.2;
            auto constant_radius = [=](RealNum t) { return radius; };
            bool is_static = (dist(my_engine()) > 0.2);

            if ((center - Vec3(4.0, 0.2, 0.0)).SquaredNorm() > 0.9 * 0.9)
            {
                if (mat_choice < 0.65)
                {
                    Vec3 albedo(dist(my_engine()) * dist(my_engine()),
                                dist(my_engine()) * dist(my_engine()),
                                dist(my_engine()) * dist(my_engine()));
                    if (is_static)
                    {
                        world_.Add(std::make_shared<Sphere<Vec3,RealNum> >(
                            center, radius, std::make_shared<Lambertian>(albedo))
                        );
                    }
                    else
                    {
                        world_.Add(std::make_shared<Sphere<decltype(moving_center), decltype(constant_radius)> >(
                            moving_center, constant_radius, std::make_shared<Lambertian>(albedo))
                        );
                    }
                }
                else if (mat_choice < 0.85)
                {
                    Vec3 albedo(0.5*(1.0 + dist(my_engine())),
                                0.5*(1.0 + dist(my_engine())),
                                0.5*(1.0 + dist(my_engine())));
                    if (is_static)
                    {
                        world_.Add(std::make_shared<Sphere<Vec3,RealNum> >(
                            center, radius, std::make_shared<Metal>(albedo, 0.5*dist(my_engine())))
                        );
                    }
                    else
                    {
                        world_.Add(std::make_shared<Sphere<decltype(moving_center), decltype(constant_radius)> >(
                            moving_center, constant_radius, std::make_shared<Metal>(albedo, 0.5*dist(my_engine())))
                        );
                    }
                }
                else
                {
                    if (is_static)
                    {
                        world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
                            center, radius, std::make_shared<Dielectric>(1.5))
                        );
                    }
                    else
                    {
                        world_.Add(std::make_shared<Sphere<decltype(moving_center), decltype(constant_radius)> >(
                            moving_center, constant_radius, std::make_shared<Dielectric>(1.5))
                        );
                    }
                }
            }
        }
    }
}

} // namespace glancy

} // namespace plemma
