//
// Created by penguinlemma on 26.05.19.
//

#pragma once

#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <sstream>

#include "catch.hpp"
#include "testing_constants.hpp"
#include "vec3_test.hpp"

#include "vec3.hpp"
#include "ray.hpp"
#include "utilities.hpp"

namespace plemma::glancy
{

class RayRandomGenerator : public Catch::Generators::IGenerator<Ray>
{
public:
    RayRandomGenerator()
    {
        static_cast<void>(next());
    }

    RayRandomGenerator(RealNum min, RealNum max, RealNum tmin, RealNum tmax) :
            origen_gen_(min, max), direction_gen_(min, max), dist_(tmin, tmax)
    {
        static_cast<void>(next());
    }

    Ray const& get() const override;
    bool next() override
    {
        if(!origen_gen_.next()) return false;

        // Direction needs to be a non-zero (sufficiently big) vector
        Vec3 dir;
        while(!CanVec3BeUsedToDivide(dir))
        {
            if(!direction_gen_.next()) return false;
            dir = direction_gen_.get();
        }

        current_ = Ray(origen_gen_.get(), dir, dist_(rand_engine()));
        return true;
    }
private:
    std::default_random_engine& rand_engine()
    {
        static std::default_random_engine eng(Catch::rngSeed());

        return eng;
    }
    Ray current_;
    Vec3RandomGenerator origen_gen_;
    Vec3RandomGenerator direction_gen_;
    std::uniform_real_distribution<RealNum> dist_;
};

inline Ray const& RayRandomGenerator::get() const
{
    return current_;
}

inline Catch::Generators::GeneratorWrapper<Ray> RandomRay()
{
    return Catch::Generators::GeneratorWrapper<Ray>(std::make_unique<RayRandomGenerator>());
}
inline Catch::Generators::GeneratorWrapper<Ray> RandomRay(RealNum min, RealNum max)
{
    return Catch::Generators::GeneratorWrapper<Ray>(std::make_unique<RayRandomGenerator>(min, max, Real(0.0), Real(1.0)));
}

inline Catch::Generators::GeneratorWrapper<Ray> RandomRay(RealNum min, RealNum max, RealNum tmin, RealNum tmax)
{
    return Catch::Generators::GeneratorWrapper<Ray>(std::make_unique<RayRandomGenerator>(min, max, tmin, tmax));
}

} // namespace plemma::glancy
