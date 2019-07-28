//
// Created by penguinlemma on 26.05.19.
//

#include "ray_random_generator.hpp"

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

TEST_CASE("Reflect : Vec3 x Vec3 -> Vec3", "[Ray]")
{
    Vec3 randvec = GENERATE(take(10, filter(CanVec3BeUsedToDivide,RandomFiniteVec3(-10.0, 10.0))));

    SECTION("Zero vector is not reflected")
    {
        Vec3 normal{randvec};
        CHECK( Reflect(Vec3{}, normal) == Vec3{} );
    }

    SECTION("If normal is 0, reflection is identity")
    {
        CHECK( Reflect(randvec, Vec3{}) == randvec );
    }

    SECTION("Reflected vector and original vector have the same norm")
    {
        Vec3 normal = GENERATE(take(10, filter(CanVec3BeUsedToDivide,RandomFiniteVec3(-1.0, 1.0))));
        normal.Normalize();
        CHECK( Reflect(randvec, normal).Norm()
            == Approx(randvec.Norm()).epsilon(tconst::kRelativeToleranceEqualityCheck) );
    }

    SECTION("Calculation is correct in the axis' planes")
    {
        RealNum a_positive = GENERATE(take(10, random(0.0, 10.0)));
        RealNum b_positive = GENERATE(take(10, random(0.0, 10.0)));
        RealNum sign = GENERATE(Real(-1), Real(1));
        SECTION("Vectors inside plane xy")
        {
            CHECK_THAT(
                Reflect(sign * Vec3{-a_positive, b_positive, Real(0)}, sign * Vec3{Real(1), Real(0), Real(0)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{a_positive, b_positive, Real(0)}) );
            CHECK_THAT(
                Reflect(sign * Vec3{-a_positive, -b_positive, Real(0)}, sign * Vec3{Real(1), Real(0), Real(0)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{a_positive, -b_positive, Real(0)}) );
            CHECK_THAT(
                Reflect(sign * Vec3{a_positive, -b_positive, Real(0)}, sign * Vec3{Real(0), Real(1), Real(0)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{a_positive, b_positive, Real(0)}) );
            CHECK_THAT(
                Reflect(sign * Vec3{-a_positive, -b_positive, Real(0)}, sign * Vec3{Real(0), Real(1), Real(0)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{-a_positive, b_positive, Real(0)}) );
        }

        SECTION("Vectors inside plane xz")
        {
            CHECK_THAT(
                Reflect(sign * Vec3{-a_positive, Real(0), b_positive}, sign * Vec3{Real(1), Real(0), Real(0)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{a_positive, Real(0), b_positive}) );
            CHECK_THAT(
                Reflect(sign * Vec3{-a_positive, Real(0), -b_positive}, sign * Vec3{Real(1), Real(0), Real(0)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{a_positive, Real(0), -b_positive}) );
            CHECK_THAT(
                Reflect(sign * Vec3{a_positive, Real(0), -b_positive}, sign * Vec3{Real(0), Real(0), Real(1)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{a_positive, Real(0), b_positive}) );
            CHECK_THAT(
                Reflect(sign * Vec3{-a_positive, Real(0), -b_positive}, sign * Vec3{Real(0), Real(0), Real(1)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{-a_positive, Real(0), b_positive}) );
        }

        SECTION("Vectors inside plane yz")
        {
            CHECK_THAT(
                Reflect(sign * Vec3{Real(0), -a_positive, b_positive}, sign * Vec3{Real(0), Real(1), Real(0)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{Real(0), a_positive, b_positive}) );
            CHECK_THAT(
                Reflect(sign * Vec3{Real(0), -a_positive, -b_positive}, sign * Vec3{Real(0), Real(1), Real(0)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{Real(0), a_positive, -b_positive}) );
            CHECK_THAT(
                Reflect(sign * Vec3{Real(0), a_positive, -b_positive}, sign * Vec3{Real(0), Real(0), Real(1)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{Real(0), a_positive, b_positive}) );
            CHECK_THAT(
                Reflect(sign * Vec3{Real(0), -a_positive, -b_positive}, sign * Vec3{Real(0), Real(0), Real(1)}),
                IsComponentWiseApprox<Vec3>(
                    tconst::kRelativeToleranceEqualityCheck,
                    sign * Vec3{Real(0), -a_positive, b_positive}) );
        }
    }
}


} // namespace plemma::glancy
