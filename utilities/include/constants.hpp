#pragma once

#include <cmath>

#include "types.hpp"

namespace plemma::glancy::constants {

RealNum const kPi = std::acos(Real(-1));
constexpr RealNum kSecondsBetweenSnapshotsForBBoxCalculation = Real(0.001);

}  // namespace plemma
