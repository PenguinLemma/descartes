#pragma once

#include<cmath>
#include<chrono>

namespace plemma
{
namespace glancy
{

typedef float RealNum;

template <typename T>
constexpr RealNum Real(T number)
{
	return static_cast<RealNum>(number);
}

namespace utilities
{

inline RealNum Schlick(RealNum cosine, RealNum refraction_index)
{
    RealNum r0 = (Real(1) - refraction_index) / (Real(1) + refraction_index);
    r0 = r0 * r0;
    return r0 + (Real(1) - r0) * std::pow((Real(1) - cosine), Real(5));
}



} // namespace utilities

} // namespace glancy

} // namespace plemma
