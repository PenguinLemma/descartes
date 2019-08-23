#pragma once

#include <memory>
#include "camera.hpp"
#include "hitable_list.hpp"
#include "vec3.hpp"

namespace plemma {
namespace glancy {

class Scene
{
  public:
    virtual void LoadWorld() = 0;
    virtual const HitableList& World() const = 0;
    virtual ~Scene() {}

  private:
    HitableList world_;
};

}  // namespace glancy

}  // namespace plemma
