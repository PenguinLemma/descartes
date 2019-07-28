#pragma once

namespace plemma::glancy
{

typedef float RealNum;

template <typename T>
constexpr RealNum Real(T number) noexcept
{
    return static_cast<RealNum>(number);
}


} // namespace plemma::glancy