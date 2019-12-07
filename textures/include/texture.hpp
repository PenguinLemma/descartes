#pragma once

#include "vec3.hpp"

namespace plemma::glancy {

class Texture
{
  public:
    [[nodiscard]] virtual Vec3 GetValue(RealNum u, RealNum v, Vec3 const& p) const = 0;
};

}  // namespace plemma::glancy
