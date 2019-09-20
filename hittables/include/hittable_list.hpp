#pragma once

#include <memory>
#include <utility>
#include <vector>
#include "hittable.hpp"
#include "ray.hpp"

namespace plemma::glancy {

class HittableList : public Hittable
{
  public:
    HittableList() = default;
    HittableList(std::vector<std::shared_ptr<Hittable> >&& data) : hittables_(data) {}
    bool Hit(Ray const& r, RealNum t_min, RealNum t_max, HitRecord& rec) const override;
    bool ComputeBoundingBox(RealNum time_from,
                            RealNum time_to,
                            AxesAlignedBoundingBox& bbox) const override;
    void Add(std::shared_ptr<Hittable>&& hittable) { hittables_.push_back(hittable); }
    [[nodiscard]] auto begin() noexcept { return hittables_.begin(); }
    [[nodiscard]] auto end() noexcept { return hittables_.end(); }
    [[nodiscard]] auto begin() const noexcept { return hittables_.begin(); }
    [[nodiscard]] auto end() const noexcept { return hittables_.end(); }

  private:
    std::vector<std::shared_ptr<Hittable> > hittables_;
};

inline bool HittableList::Hit(Ray const& r, RealNum t_min, RealNum t_max, HitRecord& rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    RealNum closest_so_far = t_max;
    for (auto const& item : hittables_) {
        // we check that the pointer is not null before calling any of the
        // member functions of the object it is supposed to be pointing at
        if (item && item->Hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

inline bool HittableList::ComputeBoundingBox(RealNum time_from,
                                             RealNum time_to,
                                             AxesAlignedBoundingBox& bbox) const
{
    if (hittables_.empty())
        return false;

    AxesAlignedBoundingBox temp_bbox;
    if (!hittables_[0]->ComputeBoundingBox(time_from, time_to, temp_bbox)) {
        return false;
    }
    else {
        bbox = temp_bbox;
    }

    for (size_t i = 1; i < hittables_.size(); ++i) {
        if (hittables_[i]->ComputeBoundingBox(time_from, time_to, temp_bbox)) {
            bbox = UnionOfAABBs(bbox, temp_bbox);
        }
        else {
            return false;
        }
    }
    return true;
}

}  // namespace plemma::glancy
