#pragma once

#include "scene.h"
#include "sphere.h"
#include "lambertian.h"
#include "dielectric.h"
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
    world_.Add(std::make_unique<Sphere>(Vec3(0.0, -1000.0, 0.0), 1000.0,
                        std::make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5))));
    world_.Add(std::make_unique<Sphere>(Vec3(0.0, 1.0, 0.0), 1.0,
                        std::make_shared<Metal>(Vec3(0.5, 0.5, 0.5), 0.9)));
}

} // namespace glancy

} // namespace plemma
