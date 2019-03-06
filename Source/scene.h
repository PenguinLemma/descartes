#pragma once

#include "hitable_list.h"
#include "camera.h"
#include "vec3.h"
#include <memory>

namespace plemma
{
namespace glancy
{

class Scene
{
public:
    virtual void LoadWorld() = 0;
    virtual const HitableList& World() const = 0;
    virtual ~Scene() {}
private:
    HitableList world_;
};

} // namespace glancy

} // namespace plemma
