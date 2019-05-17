#include "vec3_test.hpp"
#include "container_approx.hpp"

namespace plemma {

namespace glancy {

TEST_CASE("Constructors", "[Vec3]")
{
    SECTION("Default construction gives (0,0,0)")
    {
        REQUIRE(Vec3().X() == 0.0);
        REQUIRE(Vec3().Y() == 0.0);
        REQUIRE(Vec3().Z() == 0.0);
    }

    SECTION("Parametrized construction is correct")
    {
        auto x = GENERATE(take(10, random(kMinRandomGeneration, kMaxRandomGeneration)));
        auto y = GENERATE(take(10, random(kMinRandomGeneration, kMaxRandomGeneration)));
        auto z = GENERATE(take(10, random(kMinRandomGeneration, kMaxRandomGeneration)));

        auto v = Vec3(x,y,z);
        CHECK( v.X() == x );
        CHECK( v.Y() == y );
        CHECK( v.Z() == z );
        SECTION("Colour-wise getters work the same than position-wise ones")
        {
            CHECK( v.R() == x );
            CHECK( v.G() == y );
            CHECK( v.B() == z );
        }
    }
}

TEST_CASE("op + : Vec3 -> Vec3 ", "[Vec3]")
{
    SECTION("Equivalent to identity")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( +v == v );
    }
}

TEST_CASE("op - : Vec3 -> Vec3 ", "[Vec3]")
{
    SECTION("It's an involution")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( -(-v) == v );
    }

    SECTION("(0, 0, 0) is a fixed point")
    {
        CHECK( -Vec3() == Vec3() );
    }
}

TEST_CASE("op + : Vec3 x Vec3 -> Vec3", "[Vec3]")
{
    REQUIRE(Vec3().X() == 0.0);
    REQUIRE(Vec3().Y() == 0.0);
    REQUIRE(Vec3().Z() == 0.0);
    SECTION("(0 0 0) is neutral element")
    {
        Vec3 zero;
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( v + zero == v );
        CHECK( zero + v == v );
    }

    SECTION("It is associative")
    {
        auto u = GENERATE(take(100, RandomFiniteVec3()));
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( u + v == v + u );
    }

    SECTION("Every element has an opposite")
    {
        Vec3 zero;
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( v + (-v) == zero );
        CHECK( -v + v == zero );
        v += -v;
        CHECK( v == zero );
    }
}

TEST_CASE("op * : R x Vec3 -> Vec3", "[Vec3]")
{
    SECTION("1.0 is neutral element", "[Vec3]")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( 1.0 * v == v );
    }

    SECTION("Multiplication by scalar well defined in terms of +")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( 2.0 * v == v + v );
        CHECK( -1.0 * v == -v );
        Vec3 triple_v = v;
        triple_v *= 3.0;
        CHECK( triple_v == v + v + v );
        Vec3 minus2_v = v;
        minus2_v *= -2.0;
        CHECK( minus2_v == - v + (-v) );
    }

    SECTION("It is commutative")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        auto k = GENERATE(take(10, random(Real(-100.0), Real(100.0))));
        CHECK( k * v == v * k );
    }

}

TEST_CASE("op / : Vec3 x R -> Vec3", "[Vec3]")
{
    SECTION("1.0 is neutral element", "[Vec3]")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( v / 1.0 == v );
    }

    SECTION("(0, 0, 0) is a fixed point")
    {
        Vec3 zero;
        auto k = GENERATE(take(100,
            filter(CanScalarBeUsedToDivide, random(Real(-100.0), Real(100.0)))));
        CHECK( zero / k == zero );
        Vec3 zero_div_k = zero;
        zero_div_k /= k;
        CHECK( zero_div_k == zero );
    }

    SECTION("Multiplication and division by scalar are inverse")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3(10.0, 10.0)));
        auto k = GENERATE(take(100,
            filter(CanScalarBeUsedToDivide, random(Real(-100.0), Real(100.0)))));
        CHECK_THAT( (k * v) / k,
                    IsComponentWiseApprox<Vec3>(kToleranceEqualityCheck, v) );
        CHECK_THAT( k *(v / k),
                    IsComponentWiseApprox<Vec3>(kToleranceEqualityCheck, v) );
        CHECK_THAT( (v * k) / k,
                    IsComponentWiseApprox<Vec3>(kToleranceEqualityCheck, v) );
        CHECK_THAT( (v / k) * k,
                    IsComponentWiseApprox<Vec3>(kToleranceEqualityCheck, v) );
    }
}

TEST_CASE("Component wise prod * : Vec3 x Vec3 -> Vec3", "[Vec3]")
{
    SECTION("(1, 1, 1) is neutral element")
    {
        Vec3 ones(1.0, 1.0, 1.0);
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( v * ones == v );
        CHECK( ones * v == v );
    }

    SECTION("(0, 0, 0) zeroes all components")
    {
        Vec3 zero;
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( v * zero == zero );
        CHECK( zero * v == zero );
    }

    SECTION("It is commutative")
    {
        auto u = GENERATE(take(100, RandomFiniteVec3()));
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( u * v == v * u );
        Vec3 u_times_v = u;
        u_times_v *= v;
        Vec3 v_times_u = v;
        v_times_u *= u;
        CHECK( u_times_v == v_times_u );
    }

    SECTION("e_i captures i-th component")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        Vec3 e1(1.0, 0.0, 0.0);
        Vec3 e2(0.0, 1.0, 0.0);
        Vec3 e3(0.0, 0.0, 1.0);

        CHECK( e1 * v == Vec3(v.X(), 0.0, 0.0) );
        CHECK( e2 * v == Vec3(0.0, v.Y(), 0.0) );
        CHECK( e3 * v == Vec3(0.0, 0.0, v.Z()) );

        CHECK( v * e1 == Vec3(v.X(), 0.0, 0.0) );
        CHECK( v * e2 == Vec3(0.0, v.Y(), 0.0) );
        CHECK( v * e3 == Vec3(0.0, 0.0, v.Z()) );
    }
}

TEST_CASE("Component wise div / : Vec3 x Vec3 -> Vec3", "[Vec3]")
{
    SECTION("(1, 1, 1) is right neutral element")
    {
        Vec3 ones(1.0, 1.0, 1.0);
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( v / ones == v );
    }

    SECTION("(0, 0, 0) is a fixed point as left argument")
    {
        Vec3 zero;
        auto v = GENERATE(take(100, filter(CanVec3BeUsedToDivide,RandomFiniteVec3())));
        CHECK( zero / v == zero );
    }

    SECTION("Division by one-self gives (1, 1, 1)")
    {
        Vec3 ones(1.0, 1.0, 1.0);
        auto v = GENERATE(take(100, filter(CanVec3BeUsedToDivide, RandomFiniteVec3())));
        CHECK( v / v == ones );
    }

    SECTION("Comp-wise multiplication and division are inverse")
    {
        auto original = GENERATE(take(100, RandomFiniteVec3(10.0, 10.0)));
        auto operand = GENERATE(take(100, filter(CanVec3BeUsedToDivide, RandomFiniteVec3())));
        CHECK_THAT( (original * operand) / operand,
                    IsComponentWiseApprox<Vec3>(kToleranceEqualityCheck, original) );
        CHECK_THAT( (operand * original) / operand,
                    IsComponentWiseApprox<Vec3>(kToleranceEqualityCheck, original) );
        CHECK_THAT( operand * (original / operand),
                    IsComponentWiseApprox<Vec3>(kToleranceEqualityCheck, original) );
    }
}

TEST_CASE("Norm : Vec3 -> R", "[Vec3]")
{
    SECTION("(0, 0, 0) has norm 0")
    {
        CHECK( Vec3().Norm() == 0.0  );
    }

    SECTION("e_i have norm 1")
    {
        Vec3 e1(1.0, 0.0, 0.0);
        Vec3 e2(0.0, 1.0, 0.0);
        Vec3 e3(0.0, 0.0, 1.0);
        CHECK ( e1.Norm() == 1.0 );
        CHECK ( e2.Norm() == 1.0 );
        CHECK ( e3.Norm() == 1.0 );
    }

    SECTION("Positive multiplicative constants can be extracted of norm")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3(-10.0, 10.0)));
        auto k = GENERATE(take(100, random(Real(0.0), Real(10.0))));
        CHECK( (k * v).Norm() == Approx(k * v.Norm()) );
        CHECK( (v * k).Norm() == Approx(k * v.Norm()) );
    }

    SECTION("Triangler inequality")
    {
        auto u = GENERATE(take(100, RandomFiniteVec3(-10.0, 10.0)));
        auto v = GENERATE(take(100, RandomFiniteVec3(-10.0, 10.0)));
        CHECK( (u + v).Norm() <= u.Norm() + v.Norm() );
    }
}

// Methods not yet tested:
// - SquaredNorm
// - Normalize (well, there is this test below)
// - operator<<
// - operator>>
// - operator==   <-- its tests should be the very first
// - Dot
// - Cross
// - UnitVector
// - GetRandomPointInUnitBall
// - GetRandomPointInUnitDiscXY


TEST_CASE("Vector is unitary after normalizing", "[Vec3][Norm]")
{
    auto v = GENERATE(take(100, RandomFiniteVec3(-30.0, 30.0)));
    v.Normalize();
    REQUIRE(v.Norm() ==  Approx(1.0));
}

} // namespace glancy

} // namespace plemma
