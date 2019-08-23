#pragma once

#include "constants.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace plemma {
namespace glancy {

// Class that models the camera, containing its position and
// direction, time that the shutter opens and closes, its
// field of view and the distance from the focus to the lense.
class Camera
{
  public:
    Camera(Vec3 lookfrom,
           Vec3 lookat,
           Vec3 vup,
           RealNum vert_fov_deg,
           RealNum aspect,
           RealNum aperture,
           RealNum focus_dist,
           RealNum t0,
           RealNum t1)
    {
        time_open_shutter_ = t0;
        time_close_shutter_ = t1;
        lens_radius_ = aperture / Real(2);
        RealNum theta = constants::kPi * vert_fov_deg / Real(180);
        RealNum half_height = tan(theta / Real(2));
        RealNum half_width = aspect * half_height;
        origin_ = lookfrom;
        looking_direction_ = UnitVector(lookfrom - lookat);
        horizontal_normal_ = UnitVector(Cross(vup, looking_direction_));
        vertical_normal_ = Cross(looking_direction_, horizontal_normal_);
        lower_left_corner_ = origin_ - half_width * focus_dist * horizontal_normal_ -
                             half_height * focus_dist * vertical_normal_ -
                             focus_dist * looking_direction_;
        horizontal_ = Real(2) * half_width * focus_dist * horizontal_normal_;
        vertical_ = Real(2) * half_height * focus_dist * vertical_normal_;
    }

    RealNum TimeShutterOpens() const { return time_open_shutter_; }
    RealNum TimeShutterCloses() const { return time_close_shutter_; }

    // Returns a ray that passes through a random point in the camera
    // lens and point in image with coordinates u and v. This ray occurs
    // at a random instant between the time the shutter of the camera
    // opens and the time it closes.
    Ray GetRay(RealNum u, RealNum v) const
    {
        Vec3 random_dir = lens_radius_ * GetRandomPointInUnitDiscXY();
        Vec3 offset = random_dir.X() * horizontal_normal_ + random_dir.Y() * vertical_normal_;
        std::uniform_real_distribution<RealNum> dist(Real(0), Real(1));
        RealNum lambda = dist(my_engine());
        RealNum t = time_open_shutter_ + lambda * (time_close_shutter_ - time_open_shutter_);
        return Ray(origin_ + offset,
                   lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_ - offset,
                   t);
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

}  // namespace glancy

}  // namespace plemma
