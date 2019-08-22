//
// Created by victormoreno on 22.08.19.
//

#pragma once

#include <cmath>
#include <random>
#include <array>
#include <iostream>
#include "utilities.hpp"
#include "rand_engine.hpp"

namespace plemma
{
namespace glancy
{
namespace raw_nums {

class Vec3 {
public:
  typedef RealNum value_type;

  constexpr Vec3() noexcept = default;
  constexpr Vec3(RealNum a, RealNum b, RealNum c) noexcept : x_{a}, y_{b}, z_{c} {}
  constexpr RealNum X() const noexcept { return x_; }
  constexpr RealNum Y() const noexcept { return y_; }
  constexpr RealNum Z() const noexcept { return z_; }
  constexpr RealNum R() const noexcept { return x_; }
  constexpr RealNum G() const noexcept { return y_; }
  constexpr RealNum B() const noexcept { return z_; }

  constexpr const Vec3 &operator+() const noexcept { return *this; }
  constexpr Vec3 operator-() const noexcept {
    return Vec3(-x_, -y_, -z_);
  }
  constexpr RealNum operator[](int i) const noexcept { return i == 0 ? x : (i == 1 ? y : z); }
  constexpr RealNum &operator[](int i) noexcept { return i == 0 ? x : (i == 1 ? y : z); }

  constexpr Vec3 &operator+=(const Vec3 &v) noexcept;
  constexpr Vec3 &operator-=(const Vec3 &v) noexcept;
  constexpr Vec3 &operator*=(const Vec3 &v) noexcept;
  constexpr Vec3 &operator/=(const Vec3 &v) noexcept;
  constexpr Vec3 &operator*=(RealNum t) noexcept;
  constexpr Vec3 &operator/=(RealNum t) noexcept;

  // Only for testing purposes
  constexpr std::size_t size() const noexcept {
    return static_cast<std::size_t>(3);
  }

  RealNum Norm() const noexcept {
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
  }

  constexpr RealNum SquaredNorm() const noexcept {
    return x_ * x_ + y_ * y_ + z_ * z_;
  }

  void Normalize() noexcept;

private:
  RealNum x_{};
  RealNum y_{};
  RealNum z_{};
};

inline std::istream &operator>>(std::istream &is, Vec3 &v) noexcept {
  is >> v.X() >> v.Y() >> v.Z();
  return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vec3 &v) noexcept {
  os << v.X() << " " << v.Y() << " " << v.Z();
  return os;
}

constexpr bool operator==(Vec3 const &u, Vec3 const &v) {
  return (u.X() == v.X()) && (u.Y() == v.Y()) && (u.Z() == v.Z());
}

inline void Vec3::Normalize() noexcept {
  RealNum norm_inverse =
      Real(1) / std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
  comp_[0] *= norm_inverse;
  comp_[1] *= norm_inverse;
  comp_[2] *= norm_inverse;
}

constexpr Vec3 operator+(const Vec3 &u, const Vec3 &v) noexcept {
  return Vec3(u.X() + v.X(), u.Y() + v.Y(), u.Z() + v.Z());
}

constexpr Vec3 operator-(const Vec3 &u, const Vec3 &v) noexcept {
  return Vec3(u.X() - v.X(), u.Y() - v.Y(), u.Z() - v.Z());
}

constexpr Vec3 operator*(const Vec3 &u, const Vec3 &v) noexcept {
  return Vec3(u.X() * v.X(), u.Y() * v.Y(), u.Z() * v.Z());
}

constexpr Vec3 operator/(const Vec3 &u, const Vec3 &v) noexcept {
  return Vec3(u.X() / v.X(), u.Y() / v.Y(), u.Z() / v.Z());
}

constexpr Vec3 operator*(RealNum t, const Vec3 &v) noexcept {
  return Vec3(t * v.X(), t * v.Y(), t * v.Z());
}

constexpr Vec3 operator/(const Vec3 &v, const RealNum t) noexcept {
  return Vec3(v.X() / t, v.Y() / t, v.Z() / t);
}

constexpr Vec3 operator*(const Vec3 &v, const RealNum t) noexcept {
  return Vec3(v.X() * t, v.Y() * t, v.Z() * t);
}

constexpr RealNum Dot(const Vec3 &u, const Vec3 &v) noexcept {
  return u.X() * v.X() + u.Y() * v.Y() + u.Z() * v.Z();
}

constexpr Vec3 Cross(const Vec3 &u, const Vec3 &v) noexcept {
  return Vec3(u.Y() * v.Z() - u.Z() * v.Y(), u.Z() * v.X() - u.X() * v.Z(),
              u.X() * v.Y() - u.Y() * v.X());
}

constexpr Vec3 &Vec3::operator+=(const Vec3 &v) noexcept {
  x_ += v.X();
  y_ += v.Y();
  z_ += v.Z();
  return *this;
}

constexpr Vec3 &Vec3::operator-=(const Vec3 &v) noexcept {
  x_ -= v.X();
  y_ -= v.Y();
  z_ -= v.Z();
  return *this;
}

constexpr Vec3 &Vec3::operator*=(const Vec3 &v) noexcept {
  x_ *= v.X();
  y_ *= v.Y();
  z_ *= v.Z();
  return *this;
}

constexpr Vec3 &Vec3::operator/=(const Vec3 &v) noexcept {
  x_ /= v.X();
  y_ /= v.Y();
  z_ /= v.Z();
  return *this;
}

constexpr Vec3 &Vec3::operator*=(const RealNum t) noexcept {
  x_ *= t;
  y_ *= t;
  z_ *= t;
  return *this;
}

constexpr Vec3 &Vec3::operator/=(const RealNum t) noexcept {
  RealNum tinv = Real(1) / t;
  x_ *= tinv;
  y_ *= tinv;
  z_ *= tinv;
  return *this;
}

inline Vec3 UnitVector(Vec3 v) noexcept { return v / v.Norm(); }

inline Vec3 GetRandomPointInUnitBall() noexcept {
  Vec3 p(Real(1), Real(1), Real(1));
  std::uniform_real_distribution<RealNum> distribution(Real(-1), Real(1));
  while (!(p.SquaredNorm() < Real(1))) {
    p = Vec3(distribution(my_engine()), distribution(my_engine()),
             distribution(my_engine()));
  }
  return p;
}

inline Vec3 GetRandomPointInUnitDiscXY() noexcept {
  Vec3 p(Real(1), Real(1), Real(1));
  std::uniform_real_distribution<RealNum> distribution(Real(-1), Real(1));
  while (!(p.SquaredNorm() < Real(1))) {
    p = Vec3(distribution(my_engine()), distribution(my_engine()), Real(0));
  }
  return p;
}

} // namespace raw_nums

} // namespace glancy

} // namespace plemma

