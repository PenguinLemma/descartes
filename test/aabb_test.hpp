//
// Created by penguinlemma on 25.05.19.
//

#pragma once

#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <sstream>

#include <catch2/catch.hpp>
#include "testing_constants.hpp"
#include "vec3_test.hpp"
#include "ray_test.hpp"

#include "axes_aligned_bounding_box.hpp"

namespace plemma::glancy
{

constexpr auto IsAABBNonEmpty = [](AxesAlignedBoundingBox const& bbox) {
    return bbox.Maxima().X() > bbox.Minima().X()
        && bbox.Maxima().Y() > bbox.Minima().Y()
        && bbox.Maxima().Z() > bbox.Minima().Z(); };

constexpr auto IsAABBEmpty = [](AxesAlignedBoundingBox const& bbox) {
    return !IsAABBNonEmpty(bbox); };

class AABBRandomGenerator : public Catch::Generators::IGenerator<AxesAlignedBoundingBox>
{
public:
    AABBRandomGenerator()
    {
        static_cast<void>(next());
    }

    AABBRandomGenerator(RealNum min, RealNum max) :
            min_gen_(min, max), max_gen_(min, max)
    {
        static_cast<void>(next());
    }

    AxesAlignedBoundingBox const& get() const override;
    bool next() override
    {
        if(!min_gen_.next()) return false;
        if(!max_gen_.next()) return false;
        current_ = AxesAlignedBoundingBox(min_gen_.get(), max_gen_.get());
        return true;
    }
private:
    AxesAlignedBoundingBox current_;
    Vec3RandomGenerator min_gen_;
    Vec3RandomGenerator max_gen_;
};

inline AxesAlignedBoundingBox const& AABBRandomGenerator::get() const
{
    return current_;
}

inline Catch::Generators::GeneratorWrapper<AxesAlignedBoundingBox> RandomFiniteAABB()
{
    return Catch::Generators::GeneratorWrapper<AxesAlignedBoundingBox>(std::make_unique<AABBRandomGenerator>());
}

inline Catch::Generators::GeneratorWrapper<AxesAlignedBoundingBox> RandomFiniteAABB(RealNum min, RealNum max)
{
    return Catch::Generators::GeneratorWrapper<AxesAlignedBoundingBox>(std::make_unique<AABBRandomGenerator>(min, max));
}

} // namespace plemma::glancy
