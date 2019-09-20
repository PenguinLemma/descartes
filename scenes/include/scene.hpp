#pragma once

#include <memory>
#include "camera.hpp"
#include "hittable_list.hpp"
#include "vec3.hpp"

namespace plemma::glancy {

class Scene
{
  public:
    virtual void LoadWorld() noexcept = 0;
    [[nodiscard]] virtual HittableList const& World() const noexcept = 0;
    virtual ~Scene() = default;

  private:
    HittableList world_;
};

}  // namespace plemma::glancy
