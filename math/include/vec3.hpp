#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <random>
#include "rand_engine.hpp"
#include "utilities.hpp"

namespace plemma::glancy {

class Vec3
{
  public:
    typedef RealNum value_type;

    constexpr Vec3() noexcept = default;
    constexpr Vec3(RealNum a, RealNum b, RealNum c) noexcept : comp_{a, b, c} {}
    [[nodiscard]] constexpr RealNum X() const noexcept { return comp_[0]; }
    [[nodiscard]] constexpr RealNum Y() const noexcept { return comp_[1]; }
    [[nodiscard]] constexpr RealNum Z() const noexcept { return comp_[2]; }
    [[nodiscard]] constexpr RealNum R() const noexcept { return comp_[0]; }
    [[nodiscard]] constexpr RealNum G() const noexcept { return comp_[1]; }
    [[nodiscard]] constexpr RealNum B() const noexcept { return comp_[2]; }

    constexpr const Vec3& operator+() const noexcept { return *this; }
    constexpr Vec3 operator-() const noexcept { return Vec3(-comp_[0], -comp_[1], -comp_[2]); }
    constexpr RealNum operator[](int i) const noexcept { return comp_[i]; }
    constexpr RealNum& operator[](int i) noexcept { return comp_[i]; }
    [[nodiscard]] constexpr auto begin() noexcept { return comp_.begin(); }
    [[nodiscard]] constexpr auto end() noexcept { return comp_.end(); }
    [[nodiscard]] constexpr auto begin() const noexcept { return comp_.begin(); }
    [[nodiscard]] constexpr auto end() const noexcept { return comp_.end(); }

    constexpr Vec3& operator+=(Vec3 const& v) noexcept;
    constexpr Vec3& operator-=(Vec3 const& v) noexcept;
    constexpr Vec3& operator*=(Vec3 const& v) noexcept;
    constexpr Vec3& operator/=(Vec3 const& v) noexcept;
    constexpr Vec3& operator*=(RealNum t) noexcept;
    constexpr Vec3& operator/=(RealNum t) noexcept;

    // Only for testing purposes
    [[nodiscard]] constexpr std::size_t size() const noexcept { return static_cast<std::size_t>(3); }

    [[nodiscard]] RealNum Norm() const noexcept
    {
        return std::sqrt(comp_[0] * comp_[0] + comp_[1] * comp_[1] + comp_[2] * comp_[2]);
    }

    [[nodiscard]] constexpr RealNum SquaredNorm() const noexcept
    {
        return comp_[0] * comp_[0] + comp_[1] * comp_[1] + comp_[2] * comp_[2];
    }

    void Normalize() noexcept;

  private:
    std::array<RealNum, 3> comp_{};
};

inline std::istream& operator>>(std::istream& is, Vec3& v) noexcept
{
    is >> v[0] >> v[1] >> v[2];
    return is;
}

inline std::ostream& operator<<(std::ostream& os, Vec3 const& v) noexcept
{
    os << v[0] << " " << v[1] << " " << v[2];
    return os;
}

constexpr bool operator==(Vec3 const& u, Vec3 const& v)
{
    return (u[0] == v[0]) && (u[1] == v[1]) && (u[2] == v[2]);
}

inline void Vec3::Normalize() noexcept
{
    RealNum norm_inverse =
        Real(1) / std::sqrt(comp_[0] * comp_[0] + comp_[1] * comp_[1] + comp_[2] * comp_[2]);
    comp_[0] *= norm_inverse;
    comp_[1] *= norm_inverse;
    comp_[2] *= norm_inverse;
}

constexpr Vec3 operator+(Vec3 const& u, Vec3 const& v) noexcept
{
    return Vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

constexpr Vec3 operator-(Vec3 const& u, Vec3 const& v) noexcept
{
    return Vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

constexpr Vec3 operator*(Vec3 const& u, Vec3 const& v) noexcept
{
    return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

constexpr Vec3 operator/(Vec3 const& u, Vec3 const& v) noexcept
{
    return Vec3(u[0] / v[0], u[1] / v[1], u[2] / v[2]);
}

constexpr Vec3 operator*(RealNum t, Vec3 const& v) noexcept
{
    return Vec3(t * v[0], t * v[1], t * v[2]);
}

constexpr Vec3 operator/(Vec3 const& v, RealNum t) noexcept
{
    return Vec3(v[0] / t, v[1] / t, v[2] / t);
}

constexpr Vec3 operator*(Vec3 const& v, RealNum t) noexcept
{
    return Vec3(v[0] * t, v[1] * t, v[2] * t);
}

constexpr RealNum Dot(Vec3 const& u, Vec3 const& v) noexcept
{
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

constexpr Vec3 Cross(Vec3 const& u, Vec3 const& v) noexcept
{
    return Vec3(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]);
}

constexpr Vec3& Vec3::operator+=(Vec3 const& v) noexcept
{
    comp_[0] += v[0];
    comp_[1] += v[1];
    comp_[2] += v[2];
    return *this;
}

constexpr Vec3& Vec3::operator-=(Vec3 const& v) noexcept
{
    comp_[0] -= v[0];
    comp_[1] -= v[1];
    comp_[2] -= v[2];
    return *this;
}

constexpr Vec3& Vec3::operator*=(Vec3 const& v) noexcept
{
    comp_[0] *= v[0];
    comp_[1] *= v[1];
    comp_[2] *= v[2];
    return *this;
}

constexpr Vec3& Vec3::operator/=(Vec3 const& v) noexcept
{
    comp_[0] /= v[0];
    comp_[1] /= v[1];
    comp_[2] /= v[2];
    return *this;
}

constexpr Vec3& Vec3::operator*=(RealNum t) noexcept
{
    comp_[0] *= t;
    comp_[1] *= t;
    comp_[2] *= t;
    return *this;
}

constexpr Vec3& Vec3::operator/=(RealNum t) noexcept
{
    RealNum tinv = Real(1) / t;
    comp_[0] *= tinv;
    comp_[1] *= tinv;
    comp_[2] *= tinv;
    return *this;
}

inline Vec3 UnitVector(Vec3 const& v) noexcept
{
    return v / v.Norm();
}

inline Vec3 GetRandomPointInUnitBall() noexcept
{
    Vec3 p(Real(1), Real(1), Real(1));
    std::uniform_real_distribution<RealNum> distribution(Real(-1), Real(1));
    while (!(p.SquaredNorm() < Real(1))) {
        p = Vec3(distribution(my_engine()), distribution(my_engine()), distribution(my_engine()));
    }
    return p;
}

inline Vec3 GetRandomPointInUnitDiscXY() noexcept
{
    Vec3 p(Real(1), Real(1), Real(1));
    std::uniform_real_distribution<RealNum> distribution(Real(-1), Real(1));
    while (!(p.SquaredNorm() < Real(1))) {
        p = Vec3(distribution(my_engine()), distribution(my_engine()), Real(0));
    }
    return p;
}

}  // namespace plemma::glancy
