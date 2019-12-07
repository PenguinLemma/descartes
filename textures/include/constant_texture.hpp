#pragma once

#include "texture.hpp"

namespace plemma::glancy {

class ConstantTexture : public Texture
{
  public:
    explicit ConstantTexture(Vec3 const& color) : color_(color) {}
    [[nodiscard]] Vec3 GetValue([[maybe_unused]] RealNum u,
                  [[maybe_unused]] RealNum v,
                  [[maybe_unused]] Vec3 const& p) const noexcept override
    {
        return color_;
    }

  private:
    Vec3 color_;
};

}  // namespace plemma::glancy
