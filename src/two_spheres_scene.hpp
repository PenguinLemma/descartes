#pragma once

#include "scene.hpp"
#include "sphere.hpp"
#include "lambertian.hpp"
#include "dielectric.hpp"
#include <memory>

namespace plemma
{
namespace glancy
{

class TwoSpheresScene : public Scene
{
public:
    void LoadWorld() override;
    virtual const HitableList& World() const override { return world_; }
    ~TwoSpheresScene() {}
private:
    HitableList world_;
};

void TwoSpheresScene::LoadWorld()
{
    Vec3 center_from(0.0, 1.0, 1.0);
    Vec3 center_to(0.0, 1.1, 1.0);
    auto center = [=](RealNum t){ return center_from + t * (center_to - center_from); };
    auto radius = [](RealNum t){ return 1.0; };
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(Vec3(0.0, -1000.0, 0.0), 1000.0,
                        std::make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5))));
    world_.Add(std::make_shared<Sphere<decltype(center), decltype(radius)> >(center, radius,
                        std::make_shared<Metal>(Vec3(0.5, 0.5, 0.5), 0.9)));
}

} // namespace glancy

} // namespace plemma
