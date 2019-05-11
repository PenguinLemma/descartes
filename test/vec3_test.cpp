#include "vec3_test.hpp"

namespace plemma {

namespace glancy {

bool AreApproxEqual(RealNum a, RealNum b, RealNum tol)
{
    return std::abs(b - a) < tol;
}



TEST_CASE("(0 0 0) is neutral element", "[Vec3][op +:Vec3 x Vec3 -> Vec3]")
{
    Vec3 zero(0.0, 0.0, 0.0);
    auto v = GENERATE(take(100, RandomFiniteVec3()));
    REQUIRE( v + zero == v );
    REQUIRE( zero + v == v );
}

TEST_CASE("Operation + is associative", "[Vec3][op +:Vec3 x Vec3 -> Vec3]")
{
    auto u = GENERATE(take(100, RandomFiniteVec3()));
    auto v = GENERATE(take(100, RandomFiniteVec3()));
    REQUIRE( u + v == v + u );
}

TEST_CASE("Every element has an opposite", "[Vec3][op +:Vec3 x Vec3 -> Vec3]")
{
    Vec3 zero(0.0, 0.0, 0.0);
    auto v = GENERATE(take(100, RandomFiniteVec3()));
    REQUIRE( v + (-v) == zero );
    v += -v;
    REQUIRE( v == zero );
}

TEST_CASE("1.0 is neutral element", "[Vec3][op *: R x Vec3 -> Vec3]")
{
    auto v = GENERATE(take(100, RandomFiniteVec3()));
    REQUIRE( 1.0 * v == v );
}

TEST_CASE("Multiplication by scalar well defined in terms of +", "[Vec3][op *: R x Vec3 -> Vec3]")
{
    auto v = GENERATE(take(100, RandomFiniteVec3()));
    REQUIRE( 2.0 * v == v + v );
}

TEST_CASE("Vector is unitary after normalizing", "[Vec3][Norm]")
{
    auto v = GENERATE(take(100, RandomFiniteVec3(-30.0, 30.0)));
    v.Normalize();
    REQUIRE(AreApproxEqual(v.Norm(), 1.0, kToleranceEqualityCheck));
}

} // namespace glancy

} // namespace plemma
