#pragma once

#include<cmath>
#include<chrono>

namespace plemma
{
namespace glancy
{

namespace utilities
{

inline RealNum Schlick(RealNum cosine, RealNum refraction_index)
{
    RealNum r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5.0);
}

} // namespace utilities

} // namespace glancy

} // namespace plemma
