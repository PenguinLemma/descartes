#pragma once

#include "hitable.hpp"
#include "axes_aligned_bounding_box.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace plemma
{
namespace glancy
{

typedef std::pair<AxesAlignedBoundingBox, std::shared_ptr<Hitable> > HitableInABox;

// Class that acts both as node and tree implementation.
// BoundingVolumeHierarchy will allow us to do binary search
// over the hitables with each ray by having a bbox that
// contains both children volumes/hitables as parent.
// It requires to have pre-computed AABBs for each
// hitable to avoid repeating the calculations.
class BoundingVolumeHierarchy : public Hitable
{
public:
    BoundingVolumeHierarchy() = default;
    // Constructor that builds the BVH tree from all hitables
    BoundingVolumeHierarchy(std::vector<HitableInABox>& boxed_hitables,
        RealNum t0, RealNum t1)
    :
        BoundingVolumeHierarchy(boxed_hitables,
            0, static_cast<int>(boxed_hitables.size()),
            t0, t1)
    {
    }

    // Constructor that builds the BVH tree from the hitables from
    // position 'from' to position 'to'. Essential given recursive
    // structure (and creation) of BVH tree.
    BoundingVolumeHierarchy(std::vector<HitableInABox>& boxed_hitables,
        int from, int to, RealNum t0, RealNum t1);

    // Check if ray hits the parent, in the case it does, recursively
    // call hit until reaching a leaf (where actual hitables are)
    virtual bool Hit(const Ray& r, RealNum t_min, RealNum t_max, HitRecord& rec) const override;

    // Computes AxesAlignedBoundingBox if possible and returns
    // whether it was possible or not.
    virtual bool ComputeBoundingBox(RealNum time_from, RealNum time_to, AxesAlignedBoundingBox& bbox) const override
    {
        bbox = bbox_;
        return true;
    }

private:
    AxesAlignedBoundingBox bbox_;
    std::shared_ptr<Hitable> left_child_;
    std::shared_ptr<Hitable> right_child_;
};

bool BoundingVolumeHierarchy::Hit(const Ray& r, RealNum t_min, RealNum t_max, HitRecord& rec) const
{
    if (!bbox_.Hit(r, t_min, t_max))
    {
        return false;
    }

    HitRecord left_rec, right_rec;
    bool hits_left_child = left_child_->Hit(r, t_min, t_max, left_rec);
    bool hits_right_child = right_child_->Hit(r, t_min, t_max, right_rec);

    if(hits_left_child && hits_right_child)
    {
        if (left_rec.t < right_rec.t)
            rec = left_rec;
        else
            rec = right_rec;
        return true;
    }
    else if (hits_left_child)
    {
        rec =  left_rec;
        return true;
    }
    else if (hits_right_child)
    {
        rec = right_rec;
        return true;
    }
    else
        return false;
}

BoundingVolumeHierarchy::BoundingVolumeHierarchy(
    std::vector<HitableInABox>& boxed_hitables,
    int from, int to, RealNum t0, RealNum t1)
{
    AxesAlignedBoundingBox bbox_left, bbox_right;
    int number_elements = to - from;
    if (number_elements == 1)
    {
        left_child_ = boxed_hitables[from].second;
        bbox_left = boxed_hitables[from].first;
        right_child_ = boxed_hitables[from].second;
        bbox_right = boxed_hitables[from].first;
    }
    else if(number_elements == 2)
    {
        left_child_ = boxed_hitables[from].second;
        bbox_left = boxed_hitables[from].first;
        right_child_ = boxed_hitables[from+1].second;
        bbox_right = boxed_hitables[from+1].first;
    }
    else
    {
        left_child_ = std::make_shared<BoundingVolumeHierarchy>(
            boxed_hitables,
            from, from + number_elements/2,
            t0, t1);
        left_child_->ComputeBoundingBox(t0, t1, bbox_left);
        right_child_ = std::make_shared<BoundingVolumeHierarchy>(
            boxed_hitables,
            from + number_elements/2, to,
            t0, t1);
        right_child_->ComputeBoundingBox(t0, t1, bbox_right);
    }
    bbox_ = UnionOfAABBs(bbox_left, bbox_right);
}


} // namespace glancy

} // namespace plemma
