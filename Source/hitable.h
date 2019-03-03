#pragma once

#include "ray.h"
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
    virtual ~Hitable() {}
};

} // namespace glancy

} // namespace plemma
