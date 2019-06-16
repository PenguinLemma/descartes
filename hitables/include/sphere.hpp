#pragma once

#include "constants.hpp"
#include "hitable.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "axes_aligned_bounding_box.hpp"
#include <cmath>
#include <memory>
#include <numeric>
#include <vector>

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
    virtual bool ComputeBoundingBox(RealNum time_from, RealNum time_to, AxesAlignedBoundingBox& bbox) const override;

private:
    Center center_;
    Radius radius_;
    std::shared_ptr<Material> material_;
};

// Returns the smallest AABB containing a static sphere
inline AxesAlignedBoundingBox ComputeAABBForFixedSphere(const Vec3& center, RealNum radius)
{
    return AxesAlignedBoundingBox(center - Vec3(radius, radius, radius),
                                  center + Vec3(radius, radius, radius));
}

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

    if (discriminant > Real(0))
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

    if (discriminant > Real(0))
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

template <typename Center, typename Radius>
bool Sphere<Center, Radius>::ComputeBoundingBox(RealNum time_from, RealNum time_to, AxesAlignedBoundingBox& bbox) const
{
    size_t number_snapshots = static_cast<size_t>(
        std::ceil((time_to - time_from) / constants::kSecondsBetweenSnapshotsForBBoxCalculation));
    std::vector<RealNum> times(number_snapshots);
    RealNum prev_to_start = time_from - constants::kSecondsBetweenSnapshotsForBBoxCalculation;
    std::generate(std::begin(times), std::end(times),
        [t = prev_to_start]() mutable { return t + constants::kSecondsBetweenSnapshotsForBBoxCalculation; }
    );

    // Compute the maximum displacement of the center between
    // snapshots to add that distance to the radius as tolerance
    // That way we might get bigger bboxes but we cover for the
    // posibility of weird movements outliers
    RealNum max_distance = 0.0;
    for (size_t i = 1; i < number_snapshots; ++i)
    {
        RealNum distance = (center_(times[i]) - center_(times[i-1])).Norm();
        if(distance > max_distance)
            max_distance = distance;
    }

    // Compute union of first and last snapshot as the might
    // be the most distant.
    bbox = UnionOfAABBs(
        ComputeAABBForFixedSphere(center_(times[0]), radius_(times[0]) + max_distance),
        ComputeAABBForFixedSphere(center_(times[number_snapshots - 1]),
                                  radius_(times[number_snapshots - 1]) + max_distance)
    );

    for (size_t i = 1; i < number_snapshots - 1; ++i)
    {
        bbox = UnionOfAABBs(
            bbox,
            ComputeAABBForFixedSphere(center_(times[i]), radius_(times[i]) + max_distance)
        );
    }
    return true;
}

template <>
bool Sphere<Vec3, RealNum>::ComputeBoundingBox(
    [[maybe_unused]] RealNum time_from,
    [[maybe_unused]] RealNum time_to,
    AxesAlignedBoundingBox& bbox) const
{
    bbox = ComputeAABBForFixedSphere(center_, radius_);
    return true;
}

} // namespace glancy

} // namespace plemma
