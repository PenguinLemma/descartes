//
// Created by penguinlemma on 25.05.19.
//

#include "aabb_random_generator.hpp"

namespace plemma::glancy {

TEST_CASE("Constructors : {} -> emptybox and Vec3xVec3 -> AABB", "[AABB]")
{
    SECTION("Default construction gives (0,0,0)")
    {
        AxesAlignedBoundingBox aabb{};
        REQUIRE(aabb.Minima() == Vec3{});
        REQUIRE(aabb.Maxima() == Vec3{});
    }

    SECTION("Parametrized construction is correct")
    {
        Vec3 minima = GENERATE(take(10, RandomFiniteVec3()));
        Vec3 maxima = GENERATE(take(10, RandomFiniteVec3()));

        AxesAlignedBoundingBox aabb{minima, maxima};

        REQUIRE(aabb.Minima() == minima);
        REQUIRE(aabb.Maxima() == maxima);
    }
}

TEST_CASE("Hit : AABB x Ray x RealNum x RealNum -> bool", "[AABB]")
{
    SECTION("Empty boxes can not be hit")
    {
        auto empty_bbox = GENERATE(take(10, filter(IsAABBEmpty, RandomFiniteAABB())));
        Ray ray = GENERATE(take(10, RandomRay()));

        CHECK(!empty_bbox.Hit(ray, tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration));
    }

    SECTION("Hit with empty constrains returns false")
    {
        auto bbox = GENERATE(take(10, RandomFiniteAABB()));
        Ray ray = GENERATE(take(10, RandomRay()));
        RealNum lower =
            GENERATE(take(10, random(tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration)));
        RealNum positive_delta =
            GENERATE(take(10, random(Real(0.0), tconst::kMaxRandomGeneration)));

        CHECK(!bbox.Hit(ray, lower, lower - positive_delta));
    }

    SECTION("Hit does its job rightly when objects and constrains are non-empty")
    {
        auto bbox = GENERATE(take(10, filter(IsAABBNonEmpty, RandomFiniteAABB(-30.0, 30.0))));
        auto mu1 = GENERATE(take(10, random(Real(-2.0), Real(2.0))));
        auto mu2 = GENERATE(take(10, random(Real(-2.0), Real(2.0))));
        auto mu3 = GENERATE(take(10, random(Real(-2.0), Real(2.0))));
        auto offset_origin = GENERATE(take(10, random(Real(1.0), Real(30.0))));
        Vec3 bbox_min{bbox.Minima()};
        Vec3 bbox_max{bbox.Maxima()};
        Vec3 bbox_diag_dir = bbox_max - bbox_min;
        Vec3 bbox_u1{Real(0.5) * bbox_diag_dir.X(), Real(0.0), Real(0.0)};
        Vec3 bbox_u2{Real(0.0), Real(0.5) * bbox_diag_dir.Y(), Real(0.0)};
        Vec3 bbox_u3{Real(0.0), Real(0.0), Real(0.5) * bbox_diag_dir.Z()};
        Vec3 center_bbox = Real(0.5) * (bbox_min + bbox_max);
        Vec3 center_front_face{bbox_min.X(), center_bbox.Y(), center_bbox.Z()};
        Vec3 center_back_face{bbox_max.X(), center_bbox.Y(), center_bbox.Z()};
        Vec3 center_left_face{center_bbox.X(), bbox_min.Y(), center_bbox.Z()};
        Vec3 center_right_face{center_bbox.X(), bbox_max.Y(), center_bbox.Z()};
        Vec3 center_bot_face{center_bbox.X(), center_bbox.Y(), bbox_min.Z()};
        Vec3 center_top_face{center_bbox.X(), center_bbox.Y(), bbox_max.Z()};

        auto ShouldRayHitFace = [](RealNum param1, RealNum param2) {
            return param1 > -1 && param1 < 1 && param2 > -1 && param2 < 1;
        };
        SECTION("Origin in front of bbox")
        {
            Vec3 origin = center_front_face - Vec3(offset_origin, Real(0.0), Real(0.0));
            Vec3 point_front_face = center_front_face + mu2 * bbox_u2 + mu3 * bbox_u3;
            Ray r{origin, point_front_face - origin, Real(0.5)};  // test is independent of ray time
            CHECK(bbox.Hit(r, tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration) ==
                  ShouldRayHitFace(mu2, mu3));
        }
        SECTION("Origin in the back of bbox")
        {
            Vec3 origin = center_back_face + Vec3(offset_origin, Real(0.0), Real(0.0));
            Vec3 point_back_face = center_back_face + mu2 * bbox_u2 + mu3 * bbox_u3;
            Ray r{origin, point_back_face - origin, Real(0.5)};  // test is independent of ray time
            CHECK(bbox.Hit(r, tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration) ==
                  ShouldRayHitFace(mu2, mu3));
        }
        SECTION("Origin to the left of bbox")
        {
            Vec3 origin = center_left_face - Vec3(Real(0.0), offset_origin, Real(0.0));
            Vec3 point_left_face = center_left_face + mu1 * bbox_u1 + mu3 * bbox_u3;
            Ray r{origin, point_left_face - origin, Real(0.5)};  // test is independent of ray time
            CHECK(bbox.Hit(r, tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration) ==
                  ShouldRayHitFace(mu1, mu3));
        }
        SECTION("Origin to the right of bbox")
        {
            Vec3 origin = center_right_face + Vec3(Real(0.0), offset_origin, Real(0.0));
            Vec3 point_right_face = center_right_face + mu1 * bbox_u1 + mu3 * bbox_u3;
            Ray r{origin, point_right_face - origin, Real(0.5)};  // test is independent of ray time
            CHECK(bbox.Hit(r, tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration) ==
                  ShouldRayHitFace(mu1, mu3));
        }
        SECTION("Origin to the bottom of bbox")
        {
            Vec3 origin = center_bot_face - Vec3(Real(0.0), Real(0.0), offset_origin);
            Vec3 point_bot_face = center_bot_face + mu1 * bbox_u1 + mu2 * bbox_u2;
            Ray r{origin, point_bot_face - origin, Real(0.5)};  // test is independent of ray time
            CHECK(bbox.Hit(r, tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration) ==
                  ShouldRayHitFace(mu1, mu2));
        }
        SECTION("Origin to the top of bbox")
        {
            Vec3 origin = center_top_face + Vec3(Real(0.0), Real(0.0), offset_origin);
            Vec3 point_top_face = center_top_face + mu1 * bbox_u1 + mu2 * bbox_u2;
            Ray r{origin, point_top_face - origin, Real(0.5)};  // test is independent of ray time
            CHECK(bbox.Hit(r, tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration) ==
                  ShouldRayHitFace(mu1, mu2));
        }
    }
}

TEST_CASE("UnionOfAABBs : AABB x AABB -> AABB", "[AABB}")
{
    SECTION("It is commutative")
    {
        auto bbox1 = GENERATE(take(10, filter(IsAABBNonEmpty, RandomFiniteAABB())));
        auto bbox2 = GENERATE(take(10, filter(IsAABBNonEmpty, RandomFiniteAABB())));
        CHECK(UnionOfAABBs(bbox1, bbox2) == UnionOfAABBs(bbox2, bbox1));
    }
    SECTION("Union of a bbox with itself is itself")
    {
        auto bbox = GENERATE(take(10, filter(IsAABBNonEmpty, RandomFiniteAABB())));
        CHECK(UnionOfAABBs(bbox, bbox) == bbox);
    }
    SECTION("Union of a bbox and another one that contains the first is the second")
    {
        auto smallbbox = GENERATE(take(10, filter(IsAABBNonEmpty, RandomFiniteAABB(-30.0, 30.0))));
        Vec3 dilatation_max = GENERATE(take(10, RandomFiniteVec3(0.0, 10.0)));
        Vec3 dilatation_min = GENERATE(take(10, RandomFiniteVec3(0.0, 10.0)));
        AxesAlignedBoundingBox bigbbox{smallbbox.Minima() - dilatation_min,
                                       smallbbox.Maxima() + dilatation_max};
        CHECK(UnionOfAABBs(smallbbox, bigbbox) == bigbbox);
    }
    SECTION(
        "Some disjoint cases where minima a maxima vectors of union are minima or maxima vector of "
        "one of the AABBs")
    {
        Vec3 minima1 = GENERATE(take(10, RandomFiniteVec3(-90.0, 30.0)));
        Vec3 diag1 = GENERATE(take(10, RandomFiniteVec3(0.0, 30.0)));
        Vec3 diag2 = GENERATE(take(10, RandomFiniteVec3(0.0, 30.0)));
        Vec3 from_max1_to_min2 = GENERATE(take(10, RandomFiniteVec3(0.0, 30.0)));
        Vec3 maxima1 = minima1 + diag1;
        Vec3 minima2 = maxima1 + from_max1_to_min2;
        Vec3 maxima2 = minima2 + diag2;
        AxesAlignedBoundingBox b1{minima1, maxima1};
        AxesAlignedBoundingBox b2{minima2, maxima2};
        CHECK(UnionOfAABBs(b1, b2) == AxesAlignedBoundingBox{minima1, maxima2});
    }
    SECTION("Some disjoint cases where resulting minima and maxima are mixture")
    {
        Vec3 minima1 = GENERATE(take(10, RandomFiniteVec3(-90.0, 30.0)));
        Vec3 diag1 = GENERATE(take(10, RandomFiniteVec3(0.0, 30.0)));
        Vec3 diag2 = GENERATE(take(10, RandomFiniteVec3(0.0, 30.0)));
        Vec3 from_max1_to_min2 = GENERATE(take(10, RandomFiniteVec3(0.0, 30.0)));
        RealNum offset_component = GENERATE(take(10, random(Real(0.0), Real(5.0))));
        Vec3 maxima1 = minima1 + diag1;
        AxesAlignedBoundingBox b1{minima1, maxima1};
        SECTION("In x direction")
        {
            Vec3 offset{
                from_max1_to_min2.X() + maxima1.X() - minima1.X() + offset_component + diag2.X(),
                Real(0.0),
                Real(0.0)};
            Vec3 minima2 = maxima1 + from_max1_to_min2 - offset;
            Vec3 maxima2 = minima2 + diag2;
            AxesAlignedBoundingBox b2{minima2, maxima2};
            CHECK(UnionOfAABBs(b1, b2) ==
                  AxesAlignedBoundingBox{Vec3{minima2.X(), minima1.Y(), minima1.Z()},
                                         Vec3{maxima1.X(), maxima2.Y(), maxima2.Z()}});
        }
        SECTION("In y direction")
        {
            Vec3 offset{
                Real(0.0),
                from_max1_to_min2.Y() + maxima1.Y() - minima1.Y() + offset_component + diag2.Y(),
                Real(0.0)};
            Vec3 minima2 = maxima1 + from_max1_to_min2 - offset;
            Vec3 maxima2 = minima2 + diag2;
            AxesAlignedBoundingBox b2{minima2, maxima2};
            CHECK(UnionOfAABBs(b1, b2) ==
                  AxesAlignedBoundingBox{Vec3{minima1.X(), minima2.Y(), minima1.Z()},
                                         Vec3{maxima2.X(), maxima1.Y(), maxima2.Z()}});
        }
        SECTION("In z direction")
        {
            Vec3 offset{
                Real(0.0),
                Real(0.0),
                from_max1_to_min2.Z() + maxima1.Z() - minima1.Z() + offset_component + diag2.Z()};
            Vec3 minima2 = maxima1 + from_max1_to_min2 - offset;
            Vec3 maxima2 = minima2 + diag2;
            AxesAlignedBoundingBox b2{minima2, maxima2};
            CHECK(UnionOfAABBs(b1, b2) ==
                  AxesAlignedBoundingBox{Vec3{minima1.X(), minima1.Y(), minima2.Z()},
                                         Vec3{maxima2.X(), maxima2.Y(), maxima1.Z()}});
        }
    }
}

TEST_CASE("operator<< : AABB -> Beautiful text", "[AABB]")
{
    std::ostringstream os;
    os << AxesAlignedBoundingBox{Vec3{-6, -4, 2}, Vec3{2, 4, 6}};
    CHECK(os.str() == "[-6, 2] x [-4, 4] x [2, 6]");
    os.str(std::string{});
    os << AxesAlignedBoundingBox{Vec3{3.45, -19, 6.0}, Vec3{3.46, 0.0, 1234}};
    CHECK(os.str() == "[3.45, 3.46] x [-19, 0] x [6, 1234]");
}

}  // namespace plemma::glancy
