#pragma once

#include <random>
#include <chrono>

namespace plemma
{
namespace glancy
{

inline std::default_random_engine& my_engine(
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())
    )
{
    static std::default_random_engine eng(seed);

    return eng;
}

} // namespace glancy

} // namespace plemma
