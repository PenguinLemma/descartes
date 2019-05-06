#pragma once

#include "vec3.hpp"

namespace plemma
{
namespace glancy
{

class Ray
{
public:
    Ray() {}
    Ray(const Vec3& origin, const Vec3& direction, RealNum t)
        : origin_(origin), direction_(direction), time_(t) {}
    inline Vec3 Origin() const { return origin_; }
    inline Vec3 Direction() const { return direction_; }
    inline RealNum Time() const { return time_; }
    inline Vec3 PointAtParameter(RealNum lambda) const { return origin_ + lambda * direction_; }

    Vec3 origin_;
    Vec3 direction_;
    RealNum time_;
};

} // namespace glancy

} // namespace plemma
