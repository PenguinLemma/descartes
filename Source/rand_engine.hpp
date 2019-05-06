#pragma once

#include <random>
#include <chrono>

namespace plemma
{
namespace glancy
{

std::default_random_engine& my_engine()
{
    static std::default_random_engine eng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

    return eng;
}

} // namespace glancy

} // namespace plemma
