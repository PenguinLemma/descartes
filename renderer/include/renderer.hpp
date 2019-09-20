#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include "bounding_volume_hierarchy.hpp"
#include "camera.hpp"
#include "image.hpp"
#include "scene.hpp"

namespace plemma::glancy {

template <typename UnaryOp>
class Renderer
{
  public:
    Renderer(UnaryOp gc, size_t h_pixels, size_t v_pixels, size_t rays_pixel, uint16_t maxd)
        : GammaCorrection(gc),
          num_horizontal_pixels_(h_pixels),
          num_vertical_pixels_(v_pixels),
          num_rays_per_pixel_(rays_pixel),
          maximum_depth_(maxd)
    {}

    void ProcessScene(Scene const& scene, Camera const& camera, Image& image) noexcept;

  private:
    [[nodiscard]] Vec3 GetColor(Hittable const& target, Ray const& r, uint16_t depth) const
        noexcept;
    void PreprocessWorld(HittableList const& world, RealNum t0, RealNum t1) noexcept;

    BoundingVolumeHierarchy ordered_world_;
    UnaryOp GammaCorrection;
    const size_t num_horizontal_pixels_;
    const size_t num_vertical_pixels_;
    const size_t num_rays_per_pixel_;
    const uint16_t maximum_depth_;
};

template <typename UnaryOp>
void Renderer<UnaryOp>::ProcessScene(Scene const& scene,
                                     Camera const& camera,
                                     Image& image) noexcept
{
    constexpr int initial_depth = 0;
    RealNum const horizontal_length = Real(num_horizontal_pixels_);
    RealNum const vertical_length = Real(num_vertical_pixels_);
    std::uniform_real_distribution<RealNum> dist(Real(0), Real(1));
    std::cout << "Pre-processing scene for faster rendering" << std::endl;
    PreprocessWorld(scene.World(), camera.TimeShutterOpens(), camera.TimeShutterCloses());
    std::cout << "0% processing completed." << std::endl;
    int percentage_completed = 0;
    int prev_percentage_written = 0;
    for (size_t j = num_vertical_pixels_; j >= 1; --j) {
        size_t index_ver = j - 1;
        for (size_t index_hor = 0; index_hor < num_horizontal_pixels_; ++index_hor) {
            Vec3 color(Real(0), Real(0), Real(0));
            for (size_t s = 0; s < num_rays_per_pixel_; ++s) {
                RealNum u = (Real(index_hor) + dist(my_engine())) / horizontal_length;
                RealNum v = (Real(index_ver) + dist(my_engine())) / vertical_length;

                Ray r = camera.GetRay(u, v);

                color += GetColor(ordered_world_, r, initial_depth);
            }

            color /= Real(num_rays_per_pixel_);

            std::transform(std::begin(color), std::end(color), std::begin(color), GammaCorrection);
            color *= Real(255.9999);
            image.PaintPixel(index_hor, index_ver, color);
        }

        percentage_completed =
            static_cast<int>(Real(100) * (Real(1) - Real(index_ver + 1) / vertical_length));
        if (static_cast<int>(percentage_completed) >= prev_percentage_written + 5) {
            int to_write = percentage_completed - (percentage_completed % 5);
            std::cout << to_write << "% processing completed." << std::endl;
            prev_percentage_written = to_write;
        }
    }

    std::cout << "100% processing completed." << std::endl;
    std::cout << std::endl;
}

template <typename UnaryOp>
Vec3 Renderer<UnaryOp>::GetColor(Hittable const& target, Ray const& r, uint16_t depth) const
    noexcept
{
    HitRecord rec;
    // We increase the minimum parameter to avoid finding out the original intersection again
    RealNum min_param = Real(0.001);
    if (target.Hit(r, min_param, std::numeric_limits<RealNum>::max(), rec)) {
        Ray scattered_ray;
        Vec3 attenuation;
        if (depth < maximum_depth_ && rec.mat->Scatter(r, rec, attenuation, scattered_ray)) {
            return attenuation * GetColor(target, scattered_ray, depth + 1);
        }
        else {
            return Vec3(Real(0), Real(0), Real(0));
        }
    }
    else {
        Vec3 unit_direction = UnitVector(r.Direction());
        RealNum t = Real(0.5) * (unit_direction.Y() + Real(1));
        return (Real(1) - t) * Vec3(Real(1), Real(1), Real(1)) +
               t * Vec3(Real(0.5), Real(0.7), Real(1));
    }
}

template <typename UnaryOp>
void Renderer<UnaryOp>::PreprocessWorld(HittableList const& world, RealNum t0, RealNum t1) noexcept
{
    std::vector<HittableInABox> boxed_hittables;
    int counter = 0;
    for (auto it = std::begin(world); it != std::end(world); ++it) {
        std::shared_ptr<Hittable> hpt = *it;
        HittableInABox& added_element = boxed_hittables.emplace_back(AxesAlignedBoundingBox(), hpt);
        hpt->ComputeBoundingBox(t0, t1, added_element.first);
        ++counter;
    }

    ordered_world_ = BoundingVolumeHierarchy(boxed_hittables, t0, t1);
}

}  // namespace plemma::glancy
