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

template <typename Center, typename Radius>
class Sphere: public Hitable
{
public:
    Sphere() {}
    Sphere(Center c, Radius r, std::shared_ptr<Material> mat) : center_(c), radius_(r), material_(mat) {};
    virtual bool Hit(const Ray& r, RealNum t_min, RealNum t_max, HitRecord& rec) const override;
    Center center_;
    Radius radius_;
    std::shared_ptr<Material> material_;
};
template<typename Center, typename Radius>
bool Sphere<Center,Radius>::Hit(const Ray& r, RealNum t_min, RealNum t_max, HitRecord& rec) const
{
    Vec3 center = center_(r.Time());
    RealNum radius = radius_(r.Time());
    Vec3 or_to_center = r.Origin() - center;
    RealNum a = Dot(r.Direction(), r.Direction());
    RealNum b = Dot(or_to_center, r.Direction());
    RealNum c = Dot(or_to_center, or_to_center) - radius * radius;
    RealNum discriminant = b*b - a*c;

    if (discriminant > 0.0)
    {
        RealNum sqrt_discriminant = std::sqrt(discriminant);
        RealNum temp = (-b - sqrt_discriminant)/a;
        if (temp < t_max && temp > t_min)
        {
            rec.t      = temp;
            rec.p      = r.PointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat    = material_;
            return true;
        }

        temp = (-b + sqrt_discriminant)/a;
        if (temp < t_max && temp > t_min)
        {
            rec.t      = temp;
            rec.p      = r.PointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat    = material_;
            return true;
        }
    }
    return false;
}


template<>
bool Sphere<Vec3, RealNum>::Hit(const Ray& r, RealNum t_min, RealNum t_max, HitRecord& rec) const
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
