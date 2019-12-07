#pragma once

#include "texture.hpp"

#include <memory>
#include <cmath>

namespace plemma::glancy {

class CheckerTexture : public Texture
{
  public:
    CheckerTexture(std::shared_ptr<Texture> t1, std::shared_ptr<Texture> t2)
        : texture1_(t1), texture2_(t2)
    {}
    // TODO: improve the checkers selection by using the parametrization of sphere
    [[nodiscard]] Vec3 GetValue(RealNum u, RealNum v, Vec3 const& p) const noexcept override
    {
        static int angle_divisions = 10;
        RealNum x_sine = std::sin(p.X() * Real(angle_divisions));
        RealNum y_sine = std::sin(p.Y() * Real(angle_divisions));
        RealNum z_sine = std::sin(p.Z() * Real(angle_divisions));
        bool const use_texture1 = x_sine * y_sine * z_sine > 0;
        return use_texture1 ? texture1_->GetValue(u, v, p) : texture2_->GetValue(u, v, p);
    }

  private:
    std::shared_ptr<Texture> texture1_;
    std::shared_ptr<Texture> texture2_;
};

}  // namespace plemma::glancy
