#include "Vec3.h"

namespace plemma
{
namespace glancy
{

class AxesAlignedBoundingBox
{
public:
    AxesAlignedBoundingBox() {}
    AxesAlignedBoundingBox(const Vec3& m, const Vec3& M) :
        minima_(m),
        maxima_(M)
    {}

    Vec3 Minima() { return minima_; }
    Vec3 Maxima() { return maxima_; }

    bool Hit(const Ray& r, RealNum param_min, RealNum param_max) const
    {
        for (int i = 0; i < 3; ++i)
        {
            RealNum inv_direction_comp = 1.0 / r.Direction()[i];
            // Calculate parameters lambda0, lambda1 such that component i of
            // r(lambda) belongs to [minima_[i], maxima_[i]] for all
            // lambda in [lambda_0, lambda_1] contained in [param_min, param_max]
            RealNum lambda_0 =  (minima_[i] - r.Origin()[i]) * inv_direction_comp;
            RealNum lambda_1 =  (maxima_[i] - r.Origin()[i]) * inv_direction_comp;
            if (inv_direction_comp < 0.0)
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

private:
    Vec3 minima_;
    Vec3 maxima_;
};

} // namespace glancy

} // namespace plemma