#pragma once

#include <array>
#include <functional>
#include <memory>
#include <utility>
#include <vector>
#include "axes_aligned_bounding_box.hpp"
#include "constants.hpp"
#include "hittable.hpp"

namespace plemma::glancy {

typedef std::pair<AxesAlignedBoundingBox, std::shared_ptr<Hittable> > HittableInABox;

// TODO: move to constexpr after moving to C++20
std::array<std::function<bool(HittableInABox const& a, HittableInABox const& b)>, 3> const
    OrderWithRespectToAxis{[](HittableInABox const& a, HittableInABox const& b) noexcept -> bool {
                               return a.first.Minima().X() < b.first.Minima().X();
                           },
                           [](HittableInABox const& a, HittableInABox const& b) noexcept -> bool {
                               return a.first.Minima().Y() < b.first.Minima().Y();
                           },
                           [](HittableInABox const& a, HittableInABox const& b) noexcept -> bool {
                               return a.first.Minima().Z() < b.first.Minima().Z();
                           }};

// Class that acts both as node and tree implementation.
// BoundingVolumeHierarchy will allow us to do binary search
// over the hittables with each ray by having a bbox that
// contains both children volumes/hittables as parent.
// It requires to have pre-computed AABBs for each
// hittable to avoid repeating the calculations.
class BoundingVolumeHierarchy : public Hittable
{
  public:
    BoundingVolumeHierarchy() = default;
    // Constructor that builds the BVH tree from all hittables
    BoundingVolumeHierarchy(std::vector<HittableInABox>& boxed_hittables, RealNum t0, RealNum t1)
        : BoundingVolumeHierarchy(boxed_hittables, 0, boxed_hittables.size(), t0, t1)
    {}

    // Constructor that builds the BVH tree from the hittables from
    // position 'from' to position 'to'. Essential given recursive
    // structure (and creation) of BVH tree.
    BoundingVolumeHierarchy(std::vector<HittableInABox>& boxed_hittables,
                            size_t from,
                            size_t to,
                            RealNum t0,
                            RealNum t1);

    // Check if ray hits the parent, in the case it does, recursively
    // call hit until reaching a leaf (where actual hittables are)
    bool Hit(Ray const& r, RealNum t_min, RealNum t_max, HitRecord& rec) const override;

    // Computes AxesAlignedBoundingBox if possible and returns
    // whether it was possible or not.
    bool ComputeBoundingBox([[maybe_unused]] RealNum time_from,
                            [[maybe_unused]] RealNum time_to,
                            AxesAlignedBoundingBox& bbox) const override
    {
        bbox = bbox_;
        return true;
    }

  private:
    int ChooseOrderingAxis([[maybe_unused]] std::vector<HittableInABox>& boxed_hittables,
                           [[maybe_unused]] size_t from,
                           [[maybe_unused]] size_t to) const;

    AxesAlignedBoundingBox bbox_;
    std::shared_ptr<Hittable> left_child_;
    std::shared_ptr<Hittable> right_child_;
};

inline bool BoundingVolumeHierarchy::Hit(Ray const& r,
                                         RealNum t_min,
                                         RealNum t_max,
                                         HitRecord& rec) const
{
    if (!bbox_.Hit(r, t_min, t_max)) {
        return false;
    }

    HitRecord left_rec, right_rec;
    bool const hits_left_child = left_child_->Hit(r, t_min, t_max, left_rec);
    bool const hits_right_child = right_child_->Hit(r, t_min, t_max, right_rec);

    if (hits_left_child && hits_right_child) {
        if (left_rec.t < right_rec.t)
            rec = left_rec;
        else
            rec = right_rec;
        return true;
    }
    else if (hits_left_child) {
        rec = left_rec;
        return true;
    }
    else if (hits_right_child) {
        rec = right_rec;
        return true;
    }
    else
        return false;
}

inline BoundingVolumeHierarchy::BoundingVolumeHierarchy(
    std::vector<HittableInABox>& boxed_hittables,
    size_t from,
    size_t to,
    RealNum t0,
    RealNum t1)
{
    AxesAlignedBoundingBox bbox_left, bbox_right;
    size_t const number_elements = to - from;
    if (number_elements == 1) {
        left_child_ = boxed_hittables[from].second;
        bbox_left = boxed_hittables[from].first;
        right_child_ = boxed_hittables[from].second;
        bbox_right = boxed_hittables[from].first;
    }
    else if (number_elements == 2) {
        left_child_ = boxed_hittables[from].second;
        bbox_left = boxed_hittables[from].first;
        right_child_ = boxed_hittables[from + 1].second;
        bbox_right = boxed_hittables[from + 1].first;
    }
    else {
        int ordering_axis = ChooseOrderingAxis(boxed_hittables, from, to);
        if (ordering_axis == 0) {
            sort(boxed_hittables.begin() + from,
                 boxed_hittables.begin() + to,
                 OrderWithRespectToAxis[ordering_axis]);
        }
        left_child_ = std::make_shared<BoundingVolumeHierarchy>(
            boxed_hittables, from, from + number_elements / 2, t0, t1);
        left_child_->ComputeBoundingBox(t0, t1, bbox_left);
        right_child_ = std::make_shared<BoundingVolumeHierarchy>(
            boxed_hittables, from + number_elements / 2, to, t0, t1);
        right_child_->ComputeBoundingBox(t0, t1, bbox_right);
    }
    bbox_ = UnionOfAABBs(bbox_left, bbox_right);
}

inline int BoundingVolumeHierarchy::ChooseOrderingAxis(
    [[maybe_unused]] std::vector<HittableInABox>& boxed_hittables,
    [[maybe_unused]] size_t from,
    [[maybe_unused]] size_t to) const
{
    std::uniform_int_distribution<> choose_axis(1, 3);
    return choose_axis(my_engine());
}

}  // namespace plemma::glancy
