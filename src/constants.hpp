#pragma once

#include <cmath>

namespace plemma
{
namespace glancy
{

namespace constants
{

const RealNum kPi = std::acos(Real(-1));
constexpr RealNum kSecondsBetweenSnapshotsForBBoxCalculation = Real(0.001);

} // namespace constants

} // namespace glancy

} // namespace plemma
