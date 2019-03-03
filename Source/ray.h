#pragma once

#include "vec3.h"

namespace plemma
{
namespace glancy
{

class Ray
{
public:
    Ray() {}
    Ray(const Vec3& origin, const Vec3& direction) : origin_(origin), direction_(direction) {}
    inline Vec3 Origin() const { return origin_; }
    inline Vec3 Direction() const { return direction_; }
    inline Vec3 PointAtParameter(RealNum t) const { return origin_ + t * direction_; }

    Vec3 origin_;
    Vec3 direction_;
};

} // namespace glancy

} // namespace plemma
