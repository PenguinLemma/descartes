#include "vec3_test.hpp"

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

    SECTION("Operation + is associative")
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
    SECTION("1.0 is neutral element", "[Vec3][op *: R x Vec3 -> Vec3]")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( 1.0 * v == v );
    }

    SECTION("Multiplication by scalar well defined in terms of +")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( 2.0 * v == v + v );
        CHECK( -1.0 * v == -v );
    }

    SECTION("It's commutative")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        auto k = GENERATE(take(10, random(Real(-100.0), Real(100.0))));
        CHECK( k * v == v * k );
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

    // I'm here :)
}

TEST_CASE("Vector is unitary after normalizing", "[Vec3][Norm]")
{
    auto v = GENERATE(take(100, RandomFiniteVec3(-30.0, 30.0)));
    v.Normalize();
    REQUIRE(v.Norm() ==  Approx(1.0));
}

} // namespace glancy

} // namespace plemma
