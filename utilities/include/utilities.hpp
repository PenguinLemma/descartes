#pragma once

#include <chrono>
#include <cmath>

#include "types.hpp"

namespace plemma::glancy::utilities {

inline RealNum Schlick(RealNum cosine, RealNum refraction_index)
{
    RealNum r0 = (Real(1) - refraction_index) / (Real(1) + refraction_index);
    r0 = r0 * r0;
    return r0 + (Real(1) - r0) * std::pow((Real(1) - cosine), Real(5));
}

}  // namespace plemma::glancy::utilities
