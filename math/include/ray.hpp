#pragma once

#include <cmath>
#include "vec3.hpp"

namespace plemma::glancy {

class Ray
{
  public:
    constexpr Ray() noexcept = default;
    constexpr Ray(Vec3 const& origin, Vec3 const& direction, RealNum t)
        : origin_(origin), direction_(direction), time_(t)
    {}
    [[nodiscard]] constexpr Vec3 Origin() const { return origin_; }
    [[nodiscard]] constexpr Vec3 Direction() const { return direction_; }
    [[nodiscard]] constexpr RealNum Time() const { return time_; }
    [[nodiscard]] constexpr Vec3 PointAtParameter(RealNum lambda) const { return origin_ + lambda * direction_; }

  private:
    Vec3 origin_{};
    Vec3 direction_{};
    RealNum time_{};
};

constexpr Vec3 Reflect(Vec3 const& v, Vec3 const& n) noexcept
{
    return v - Real(2) * Dot(v, n) * n;
}

// name "ni_over_nt" might need to be improved
inline bool Refract(Vec3 const& v, Vec3 const& n, RealNum ni_over_nt, Vec3& refracted) noexcept
{
    Vec3 const v_unit = UnitVector(v);
    RealNum const dt = Dot(v_unit, n);
    RealNum const discriminant = Real(1) - ni_over_nt * ni_over_nt * (Real(1) - dt * dt);
    if (discriminant > Real(0)) {
        refracted = ni_over_nt * (v_unit - dt * n) - std::sqrt(discriminant) * n;
        return true;
    }
    return false;
}

}  // namespace plemma::glancy
