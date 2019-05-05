#pragma once

#include "ray.h"
#include "vec3.h"
#include "constants.h"

namespace plemma
{
namespace glancy
{

class Camera
{
public:
    Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, RealNum vert_fov_deg, RealNum aspect,
           RealNum aperture, RealNum focus_dist, RealNum t0, RealNum t1)
    {
        time_open_shutter_ = t0;
        time_close_shutter_ = t1;
        lens_radius_ = aperture / 2.0;
        RealNum theta = vert_fov_deg * kPi / 180.0;
        RealNum half_height = tan(theta / 2.0);
        RealNum half_width  = aspect * half_height;
        origin_ = lookfrom;
        looking_direction_ = UnitVector(lookfrom - lookat);
        horizontal_normal_ = UnitVector(Cross(vup, looking_direction_));
        vertical_normal_   = Cross(looking_direction_, horizontal_normal_);
        lower_left_corner_ = origin_ - half_width * focus_dist * horizontal_normal_
                                     - half_height * focus_dist * vertical_normal_
                                     - focus_dist * looking_direction_;
        horizontal_ = 2.0 * half_width * focus_dist * horizontal_normal_;
        vertical_   = 2.0 * half_height * focus_dist * vertical_normal_;
    }

    RealNum TimeShutterOpens() const { return time_open_shutter_; }
    RealNum TimeShutterCloses() const { return time_close_shutter_; }

    Ray GetRay(RealNum u, RealNum v) const
    {
        Vec3 random_dir = lens_radius_ * GetRandomPointInUnitDiscXY();
        Vec3 offset = random_dir.X() * horizontal_normal_ + random_dir.Y() * vertical_normal_;
        std::uniform_real_distribution<RealNum> dist(0.0, 1.0);
        RealNum lambda = dist(my_engine());
        RealNum t = time_open_shutter_ + lambda * (time_close_shutter_ - time_open_shutter_);
        return Ray(origin_ + offset, lower_left_corner_ + u*horizontal_ + v*vertical_ - origin_ - offset, t);
    }

private:
    Vec3 lower_left_corner_;
    Vec3 horizontal_;
    Vec3 vertical_;
    Vec3 origin_;
    Vec3 horizontal_normal_;
    Vec3 vertical_normal_;
    Vec3 looking_direction_;
    RealNum lens_radius_;
    RealNum time_open_shutter_;
    RealNum time_close_shutter_;
};

} // namespace glancy

} // namespace plemma
