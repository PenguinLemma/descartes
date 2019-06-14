#pragma once

#include "ray.hpp"
#include "axes_aligned_bounding_box.hpp"
#include <memory>

namespace plemma
{
namespace glancy
{

class Material;

struct HitRecord
{
    RealNum t;
    Vec3 p;
    Vec3 normal;
    std::shared_ptr<Material> mat;
};

class Hitable
{
public:
    virtual bool Hit(const Ray& r, RealNum t_min, RealNum t_max, HitRecord& rec) const = 0;
    virtual bool ComputeBoundingBox(RealNum time_from, RealNum time_to, AxesAlignedBoundingBox& bbox) const = 0;
    virtual ~Hitable() {}
};

} // namespace glancy

} // namespace plemma
