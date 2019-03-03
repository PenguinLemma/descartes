#pragma once

#include <cmath>
#include <random>
#include <vector>
#include "rand_engine.h"

namespace plemma
{
namespace glancy
{

typedef float RealNum;


class Vec3
{
public:
    Vec3() {}
    Vec3(RealNum a, RealNum b, RealNum c) : comp_{a,b,c} {}
    inline RealNum X() const { return comp_[0]; }
    inline RealNum Y() const { return comp_[1]; }
    inline RealNum Z() const { return comp_[2]; }
    inline RealNum R() const { return comp_[0]; }
    inline RealNum G() const { return comp_[1]; }
    inline RealNum B() const { return comp_[2]; }

    inline const Vec3& operator+() const { return *this; }
    inline Vec3 operator-() const { return Vec3(-comp_[0], -comp_[1], -comp_[2]); }
    inline RealNum operator[](int i) const { return comp_[i]; }
    inline RealNum& operator[](int i) { return comp_[i]; }
    inline auto Begin() { return comp_.begin(); }
    inline auto End() { return comp_.end(); }

    inline Vec3& operator+=(const Vec3& v);
    inline Vec3& operator-=(const Vec3& v);
    inline Vec3& operator*=(const Vec3& v);
    inline Vec3& operator/=(const Vec3& v);
    inline Vec3& operator*=(const RealNum t);
    inline Vec3& operator/=(const RealNum t);

    inline RealNum Norm() const
    {
        return sqrt(comp_[0]*comp_[0] + comp_[1]*comp_[1] + comp_[2]*comp_[2]);
    }

    inline RealNum SquaredNorm() const
    {
        return comp_[0]*comp_[0] + comp_[1]*comp_[1] + comp_[2]*comp_[2];
    }

    inline void Normalize();

private:
    std::vector<RealNum> comp_{ static_cast<RealNum>(0), static_cast<RealNum>(0), static_cast<RealNum>(0) };
};


inline std::istream& operator>>(std::istream& is, Vec3& v)
{
    is >> v[0] >> v[1] >> v[2];
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
    os << v[0] << " " << v[1] << " " << v[2];
    return os;
}

inline void Vec3::Normalize()
{
    RealNum norm_inverse = static_cast<RealNum>(1.0) / std::sqrt(comp_[0]*comp_[0] + comp_[1]*comp_[1] + comp_[2]*comp_[2]);
    comp_[0] *= norm_inverse;
    comp_[1] *= norm_inverse;
    comp_[2] *= norm_inverse;
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
    return Vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
    return Vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
    return Vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline Vec3 operator/(const Vec3& u, const Vec3& v)
{
    return Vec3(u[0] / v[0], u[1] / v[1], u[2] / v[2]);
}

inline Vec3 operator*(RealNum t, const Vec3& v)
{
    return Vec3(t * v[0], t * v[1], t * v[2]);
}

inline Vec3 operator/(const Vec3& v, const RealNum t)
{
    return Vec3(v[0]/t, v[1]/t, v[2]/t);
}

inline Vec3 operator*(const Vec3& v, const RealNum t)
{
    return Vec3(v[0]*t, v[1]*t, v[2]*t);
}

inline RealNum Dot(const Vec3& u, const Vec3& v)
{
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
    return Vec3(u[1]*v[2] - u[2]*v[1],
                u[2]*v[0] - u[0]*v[2],
                u[0]*v[1] - u[1]*v[0]);
}

inline Vec3& Vec3::operator+=(const Vec3& v)
{
    comp_[0] += v[0];
    comp_[1] += v[1];
    comp_[2] += v[2];
    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& v)
{
    comp_[0] -= v[0];
    comp_[1] -= v[1];
    comp_[2] -= v[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& v)
{
    comp_[0] *= v[0];
    comp_[1] *= v[1];
    comp_[2] *= v[2];
    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& v)
{
    comp_[0] /= v[0];
    comp_[1] /= v[1];
    comp_[2] /= v[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const RealNum t)
{
    comp_[0] *= t;
    comp_[1] *= t;
    comp_[2] *= t;
    return *this;
}

inline Vec3& Vec3::operator/=(const RealNum t)
{
    RealNum tinv = static_cast<RealNum>(1.0) / t;
    comp_[0] *= tinv;
    comp_[1] *= tinv;
    comp_[2] *= tinv;
    return *this;
}

inline Vec3 UnitVector(Vec3 v)
{
    return v / v.Norm();
}

inline Vec3 GetRandomPointInUnitBall()
{
    Vec3 p(1.0f, 1.0f, 1.0f);
    std::uniform_real_distribution<RealNum> distribution(-1.0f, 1.0f);
    while (!(p.SquaredNorm() < 1.0f))
    {
        p = Vec3(distribution(my_engine()), distribution(my_engine()), distribution(my_engine()));
    }
    return p;
}

inline Vec3 GetRandomPointInUnitDiscXY()
{
    Vec3 p(1.0f, 1.0f, 0.0f);
    std::uniform_real_distribution<RealNum> distribution(-1.0f, 1.0f);
    while (!(p.SquaredNorm() < 1.0f))
    {
        p = Vec3(distribution(my_engine()), distribution(my_engine()), 0.0f);
    }
    return p;
}

inline Vec3 Reflect(const Vec3& v, const Vec3& n)
{
    return v - 2.0f * Dot(v,n)*n;
}

// name "ni_over_nt" might need to be improved
inline bool Refract(const Vec3& v, const Vec3& n, RealNum ni_over_nt, Vec3& refracted)
{
    Vec3 v_unit = UnitVector(v);
    RealNum dt = Dot(v_unit, n);
    RealNum discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
    if (discriminant > 0.0f)
    {
        refracted = ni_over_nt * (v_unit - dt * n) - sqrt(discriminant) * n;
        return true;
    }
    return false;
}

} // namespace glancy

} // namespace plemma
