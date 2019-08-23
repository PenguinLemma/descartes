#pragma once

#include <cmath>
#include "vec3.hpp"

namespace plemma {
namespace glancy {

class Ray
{
  public:
    constexpr Ray() noexcept = default;
    constexpr Ray(const Vec3& origin, const Vec3& direction, RealNum t)
        : origin_(origin), direction_(direction), time_(t)
    {}
    constexpr Vec3 Origin() const { return origin_; }
    constexpr Vec3 Direction() const { return direction_; }
    constexpr RealNum Time() const { return time_; }
    constexpr Vec3 PointAtParameter(RealNum lambda) const { return origin_ + lambda * direction_; }

  private:
    Vec3 origin_{};
    Vec3 direction_{};
    RealNum time_{};
};

constexpr Vec3 Reflect(const Vec3& v, const Vec3& n) noexcept
{
    return v - Real(2) * Dot(v, n) * n;
}

// name "ni_over_nt" might need to be improved
inline bool Refract(const Vec3& v, const Vec3& n, RealNum ni_over_nt, Vec3& refracted) noexcept
{
    Vec3 v_unit = UnitVector(v);
    RealNum dt = Dot(v_unit, n);
    RealNum discriminant = Real(1) - ni_over_nt * ni_over_nt * (Real(1) - dt * dt);
    if (discriminant > Real(0)) {
        refracted = ni_over_nt * (v_unit - dt * n) - std::sqrt(discriminant) * n;
        return true;
    }
    return false;
}

}  // namespace glancy

}  // namespace plemma
