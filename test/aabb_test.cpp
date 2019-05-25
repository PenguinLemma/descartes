//
// Created by penguinlemma on 25.05.19.
//

#include "aabb_test.hpp"

namespace plemma::glancy
{

TEST_CASE("Constructors : {} -> emptybox and Vec3xVec3 -> AABB", "[AABB]")
{
    SECTION("Default construction gives (0,0,0)")
    {
        AxesAlignedBoundingBox aabb{};
        REQUIRE( aabb.Minima() == Vec3{} );
        REQUIRE( aabb.Maxima() == Vec3{} );
    }

    SECTION("Parametrized construction is correct")
    {
        Vec3 minima = GENERATE(take(10, RandomFiniteVec3()));
        Vec3 maxima = GENERATE(take(10, RandomFiniteVec3()));

        AxesAlignedBoundingBox aabb{minima, maxima};

        REQUIRE( aabb.Minima() == minima );
        REQUIRE( aabb.Maxima() == maxima );
    }
}


} // namespace plemma::glancy

