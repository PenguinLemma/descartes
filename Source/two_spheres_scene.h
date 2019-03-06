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
    ~TwoSpheresScene() {}
private:
    HitableList world_;
};

void TwoSpheresScene::LoadWorld()
{
    world_.Add(std::make_unique<Sphere>(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
                        std::make_shared<Lambertian>(Vec3(0.5f, 0.5f, 0.5f))));
    world_.Add(std::make_unique<Sphere>(Vec3(0.0f, 1.0f, 0.0f), 1.0f,
                        std::make_shared<Dielectric>(1.5f)));
}

} // namespace glancy

} // namespace plemma
