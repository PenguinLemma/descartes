#pragma once

#include "hitable.h"
#include "ray.h"
#include <vector>
#include <memory>
#include <utility>

namespace plemma
{
namespace glancy
{

class HitableList : public Hitable
{
public:
    HitableList() {}
    HitableList(std::vector<std::shared_ptr<Hitable> >&& data) : hitables_(data) {}
    bool Hit(const Ray& r, RealNum t_min, RealNum t_max, HitRecord& rec) const override;
    bool ComputeBoundingBox(RealNum time_from, RealNum time_to, AxesAlignedBoundingBox& bbox) const override;
    void Add(std::shared_ptr<Hitable>&& hitable){ hitables_.push_back(hitable); }
    auto Begin() const { return std::begin(hitables_); }
    auto End() const { return std::end(hitables_); }

private:
    std::vector<std::shared_ptr<Hitable> > hitables_;
};

bool HitableList::Hit(const Ray& r, RealNum t_min, RealNum t_max, HitRecord& rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    RealNum closest_so_far = t_max;
    for (const auto& item : hitables_)
    {
        // we check that the pointer is not null before calling any of the
        // member functions of the object it is supposed to be pointing at
        if (item && item->Hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool HitableList::ComputeBoundingBox(RealNum time_from, RealNum time_to, AxesAlignedBoundingBox& bbox) const
{
    if (hitables_.size() < 1)
        return false;

    AxesAlignedBoundingBox temp_bbox;
    if(not hitables_[0]->ComputeBoundingBox(time_from, time_to, temp_bbox))
    {
        return false;
    }
    else
    {
        bbox = temp_bbox;
    }

    for (size_t i = 1; i < hitables_.size(); ++i)
    {
        if(hitables_[i]->ComputeBoundingBox(time_from, time_to, temp_bbox))
        {
            bbox = UnionOfAABBs(bbox, temp_bbox);
        }
        else
        {
            return false;
        }
    }
    return true;
}

} // namespace glancy

} // namespace plemma
