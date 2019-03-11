#pragma once

#include "hitable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"
#include <memory>
#include <cmath>

namespace plemma
{
namespace glancy
{

class Sphere: public Hitable
{
public:
    Sphere() {}
    Sphere(Vec3 c, RealNum r, std::shared_ptr<Material> mat) : center_(c), radius_(r), material_(mat) {};
    virtual bool Hit(const Ray& r, RealNum t_min, RealNum t_max, HitRecord& rec) const override;
    Vec3 center_;
    RealNum radius_;
    std::shared_ptr<Material> material_;
};

bool Sphere::Hit(const Ray& r, RealNum t_min, RealNum t_max, HitRecord& rec) const
{
    Vec3 or_to_center = r.Origin() - center_;
    RealNum a = Dot(r.Direction(), r.Direction());
    RealNum b = Dot(or_to_center, r.Direction());
    RealNum c = Dot(or_to_center, or_to_center) - radius_ * radius_;
    RealNum discriminant = b*b - a*c;

    if (discriminant > 0.0)
    {
        RealNum sqrt_discriminant = std::sqrt(discriminant);
        RealNum temp = (-b - sqrt_discriminant)/a;
        if (temp < t_max && temp > t_min)
        {
            rec.t      = temp;
            rec.p      = r.PointAtParameter(rec.t);
            rec.normal = (rec.p - center_) / radius_;
            rec.mat    = material_;
            return true;
        }

        temp = (-b + sqrt_discriminant)/a;
        if (temp < t_max && temp > t_min)
        {
            rec.t      = temp;
            rec.p      = r.PointAtParameter(rec.t);
            rec.normal = (rec.p - center_) / radius_;
            rec.mat    = material_;
            return true;
        }
    }
    return false;
}

} // namespace glancy

} // namespace plemma
