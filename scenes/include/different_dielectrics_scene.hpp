//
// Created by VGonzalezMoreno on 5/30/2019.
//

#pragma once

#pragma once

#include <memory>
#include "constants.hpp"
#include "dielectric.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "scene.hpp"
#include "sphere.hpp"

namespace plemma::glancy {

class DifferentDielectricsScene : public Scene
{
  public:
    void LoadWorld() override;
    virtual const HitableList& World() const override { return world_; }
    ~DifferentDielectricsScene() {}

  private:
    HitableList world_;
};

inline void DifferentDielectricsScene::LoadWorld()
{
    // All spheres will be static as this scene is to test different
    // dielectric materials
    // Add base sphere ("floor")
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3{Real(0), Real(-1e5), Real(0)},
        Real(1e5),
        std::make_shared<Lambertian>(Vec3(Real(0.0075), Real(0.3), Real(0.675)))));

    // Add one sphere for each type of dielectric (except vacuum)
    RealNum radius_big_circle{Real(5.5)};
    RealNum coordinate_odd_multiples_of_45{std::sqrt(Real(2)) * radius_big_circle / Real(2)};
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3{coordinate_odd_multiples_of_45, Real(0.5), coordinate_odd_multiples_of_45},
        Real(0.5),
        std::make_shared<Ice>()));
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3{radius_big_circle, Real(0.6), Real(0)}, Real(0.6), std::make_shared<Water>()));
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3{coordinate_odd_multiples_of_45, Real(0.7), -coordinate_odd_multiples_of_45},
        Real(0.7),
        std::make_shared<OliveOil>()));
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3{Real(0), Real(0.8), -radius_big_circle}, Real(0.8), std::make_shared<WindowGlass>()));
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3{-coordinate_odd_multiples_of_45, Real(0.9), -coordinate_odd_multiples_of_45},
        Real(0.9),
        std::make_shared<FlintGlass>()));
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3{-radius_big_circle, Real(1), Real(0)}, Real(1), std::make_shared<Sapphire>()));
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3{-coordinate_odd_multiples_of_45, Real(1.1), coordinate_odd_multiples_of_45},
        Real(1.1),
        std::make_shared<Diamond>()));

    std::uniform_real_distribution<RealNum> dist(Real(0), Real(1));
    // Add some spheres inside the "dielectric circle"
    for (int i = 0; i < 4 * static_cast<int>(radius_big_circle); ++i) {
        RealNum dist_to_origin{(radius_big_circle - Real(1)) * dist(my_engine())};
        RealNum radius{Real(0.1 + 0.35 * dist(my_engine()))};
        RealNum angle{Real(2.0) * constants::kPi * dist(my_engine())};
        Vec3 center{dist_to_origin * std::cos(angle), radius, dist_to_origin * std::sin(angle)};
        RealNum mat_choice{dist(my_engine())};
        if (mat_choice < 0.25) {
            Vec3 albedo(Real(0.5) * (Real(1) + dist(my_engine())),
                        Real(0.5) * (Real(1) + dist(my_engine())),
                        Real(0.5) * (Real(1) + dist(my_engine())));
            RealNum fuzziness{Real(0.5) * dist(my_engine())};
            world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
                center, radius, std::make_shared<Metal>(albedo, fuzziness)));
        }
        else {
            Vec3 albedo(dist(my_engine()) * dist(my_engine()),
                        dist(my_engine()) * dist(my_engine()),
                        dist(my_engine()) * dist(my_engine()));
            world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
                center, radius, std::make_shared<Lambertian>(albedo)));
        }
    }

    // Add some spheres outside the "dielectric circle"
    for (int i = 0; i < 60 * static_cast<int>(radius_big_circle); ++i) {
        RealNum dist_to_origin{radius_big_circle + Real(1) + Real(40) * dist(my_engine())};
        RealNum radius{Real(0.2 + 0.4 * dist(my_engine()))};
        RealNum angle{Real(2.0) * constants::kPi * dist(my_engine())};
        Vec3 center{dist_to_origin * std::cos(angle), radius, dist_to_origin * std::sin(angle)};
        RealNum mat_choice{dist(my_engine())};
        if (mat_choice < 0.25) {
            Vec3 albedo(Real(0.5) * (Real(1) + dist(my_engine())),
                        Real(0.5) * (Real(1) + dist(my_engine())),
                        Real(0.5) * (Real(1) + dist(my_engine())));
            RealNum fuzziness{Real(0.5) * dist(my_engine())};
            world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
                center, radius, std::make_shared<Metal>(albedo, fuzziness)));
        }
        else {
            Vec3 albedo(dist(my_engine()) * dist(my_engine()),
                        dist(my_engine()) * dist(my_engine()),
                        dist(my_engine()) * dist(my_engine()));
            world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
                center, radius, std::make_shared<Lambertian>(albedo)));
        }
    }
}

// DifferentDielectricsScene scene;
// scene.LoadWorld();
//
// size_t nx = 2000;
// size_t ny = 1500;
// Image image(nx, ny);
//
// Vec3 lookfrom(Real(16), Real(4.2), Real(9));
// Vec3 lookat(Real(0), Real(0), Real(0));
// Vec3 vertical_positive_camera{Real(0), Real(1), Real(0)};
// RealNum vertical_fov_deg{Real(30)};
// RealNum dist_to_focus = (lookfrom - lookat).Norm();
// RealNum aperture = Real(0.1);
// RealNum aspect = Real(nx) / Real(ny);
// RealNum t0{Real(0)};
// RealNum t1{Real(0.1)};

}  // namespace plemma::glancy
