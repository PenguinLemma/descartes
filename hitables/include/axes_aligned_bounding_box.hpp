#pragma once

#include <algorithm>
#include "ray.hpp"
#include "vec3.hpp"

namespace plemma::glancy {

// Class to model sub-volumes of R^3 that are the direct
// product of 3 intervals, i.e. B = [x0, x1]x[y0, y1]x[z0, z1],
// and (x, y, z) belongs to B iff x0 <= x <= x1 and y0 <= y <= y1
// and z0 <= z <= z1.
class AxesAlignedBoundingBox
{
  public:
    AxesAlignedBoundingBox() = default;
    // Constructor with minima(m) and maxima(M) as parameters.
    // Resulting box will be [m[0], M[0]]x[m[1], M[1]]x[m[2], M[2]]
    AxesAlignedBoundingBox(Vec3 const& m, Vec3 const& M) : minima_(m), maxima_(M) {}

    [[nodiscard]] constexpr Vec3 const& Minima() const noexcept { return minima_; }
    [[nodiscard]] constexpr Vec3 const& Maxima() const noexcept { return maxima_; }

    // Returns true if the ray 'r' intersects with the box for some value
    // of the parameter of the ray in [param_min, param_max]
    [[nodiscard]] bool Hit(Ray const& r, RealNum param_min, RealNum param_max) const noexcept;

  private:
    Vec3 minima_;
    Vec3 maxima_;
};

inline std::ostream& operator<<(std::ostream& os, AxesAlignedBoundingBox const& b) noexcept
{
    os << "[" << b.Minima()[0] << ", " << b.Maxima()[0] << "] x [" << b.Minima()[1] << ", "
       << b.Maxima()[1] << "] x [";
    os << b.Minima()[2] << ", " << b.Maxima()[2] << "]";
    return os;
}

inline bool AxesAlignedBoundingBox::Hit(Ray const& r, RealNum param_min, RealNum param_max) const
    noexcept
{
    for (int i = 0; i < 3; ++i) {
        RealNum const inv_direction_comp = Real(1) / r.Direction()[i];
        // Calculate parameters lambda0, lambda1 such that component i of
        // r(lambda) belongs to [minima_[i], maxima_[i]] for all
        // lambda in [lambda_0, lambda_1] contained in [param_min, param_max]
        RealNum lambda_0 = (minima_[i] - r.Origin()[i]) * inv_direction_comp;
        RealNum lambda_1 = (maxima_[i] - r.Origin()[i]) * inv_direction_comp;
        if (inv_direction_comp < Real(0))
            std::swap(lambda_0, lambda_1);

        // Calculate the intersection of the already calculated intervals
        // for the parameters with the one we just calculated
        param_min = lambda_0 > param_min ? lambda_0 : param_min;
        param_max = lambda_1 < param_max ? lambda_1 : param_max;

        // If the interval is empty, the ray doesn't hit the bounding box
        if (param_min > param_max)
            return false;
    }
    return true;
}

// Returns the union of two AABBs (i.e. the smallest AABB containing bbox1 and bbox2)
inline AxesAlignedBoundingBox UnionOfAABBs(AxesAlignedBoundingBox const& bbox1,
                                           AxesAlignedBoundingBox const& bbox2)
{
    Vec3 minima;
    Vec3 maxima;
    std::transform(std::begin(bbox1.Minima()),
                   std::end(bbox1.Minima()),
                   std::begin(bbox2.Minima()),
                   std::begin(minima),
                   [](RealNum a, RealNum b) { return std::min(a, b); });
    std::transform(std::begin(bbox1.Maxima()),
                   std::end(bbox1.Maxima()),
                   std::begin(bbox2.Maxima()),
                   std::begin(maxima),
                   [](RealNum a, RealNum b) { return std::max(a, b); });
    return AxesAlignedBoundingBox(minima, maxima);
}

}  // namespace plemma::glancy
