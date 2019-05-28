//
// Created by penguinlemma on 26.05.19.
//

#include "ray_test.hpp"

namespace plemma::glancy
{

TEST_CASE("Constructor : {} -> {r(param) = (0,0,0)}", "[Ray]")
{
    REQUIRE( Ray{}.Origin() == Vec3{} );
    REQUIRE( Ray{}.Direction() == Vec3{} );
    REQUIRE( Ray{}.Time() == 0.0 );
}

TEST_CASE("Constructor : Vec3 x Vec3 x R^+ -> Ray", "[Ray]")
{
    Vec3 origin = GENERATE(take(10, RandomFiniteVec3()));
    Vec3 direction = GENERATE(take(10, RandomFiniteVec3()));
    RealNum time = GENERATE(take(10, random(Real(0.0), Real(1.0))));

    Ray r{origin, direction, time};

    REQUIRE( r.Origin() == origin );
    REQUIRE( r.Direction() == direction );
    REQUIRE( r.Time() == time );
}

TEST_CASE("PointAtParameter : R^+ -> Vec3", "[Ray]")
{
    SECTION("Parameter = 0 returns origin")
    {
        Ray r = GENERATE(take(100, RandomRay()));
        CHECK( r.PointAtParameter(Real(0.0)) == r.Origin() );
    }

    SECTION("Parameter = 1 returns origin + direction")
    {
        Ray r = GENERATE(take(100, RandomRay()));
        CHECK( r.PointAtParameter(Real(1.0)) == r.Origin() + r.Direction() );
    }

    SECTION("Point returned is aligned with the ray")
    {
        // We are assuming that Vec3::Cross() is well tested
        Ray r = GENERATE(take(10, RandomRay(-10.0, 10.0)));
        RealNum lambda = GENERATE(take(10, random(-2.0, 2.0)));
        Vec3 origin_to_point = r.PointAtParameter(lambda) - r.Origin();

        CHECK( Cross(origin_to_point, r.Direction()).Norm()
            == Approx(0.0).margin(tconst::kAbsoluteToleranceEqualityCheckAroundZero) );
    }
}

} // namespace plemma::glancy
