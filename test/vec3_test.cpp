#include "vec3_test.hpp"
#include "container_approx.hpp"
#include "constants.hpp"

namespace plemma::glancy {

TEST_CASE("Constructors : {} -> {(0,0,0)} and R^3 -> Vec3 ", "[Vec3]")
{
    SECTION("Default construction gives (0,0,0)")
    {
        REQUIRE(Vec3().X() == 0.0);
        REQUIRE(Vec3().Y() == 0.0);
        REQUIRE(Vec3().Z() == 0.0);
    }

    SECTION("Parametrized construction is correct")
    {
        auto x = GENERATE(take(10, random(tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration)));
        auto y = GENERATE(take(10, random(tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration)));
        auto z = GENERATE(take(10, random(tconst::kMinRandomGeneration, tconst::kMaxRandomGeneration)));

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
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, v) );
        CHECK_THAT( k *(v / k),
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, v) );
        CHECK_THAT( (v * k) / k,
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, v) );
        CHECK_THAT( (v / k) * k,
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, v) );
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
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, original) );
        CHECK_THAT( (operand * original) / operand,
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, original) );
        CHECK_THAT( operand * (original / operand),
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, original) );
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
        CHECK( e1.Norm() == 1.0 );
        CHECK( e2.Norm() == 1.0 );
        CHECK( e3.Norm() == 1.0 );
    }

    SECTION("It is strictly positive for non-zero vectors")
    {
        auto v = GENERATE(take(100, filter(CanVec3BeUsedToDivide,RandomFiniteVec3())));
        CHECK( v.Norm() > 0.0 );
    }

    SECTION("Positive scalar factors can be extracted of norm")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3(-10.0, 10.0)));
        auto k = GENERATE(take(100, random(Real(0.0), Real(10.0))));
        CHECK( (k * v).Norm() == Approx(k * v.Norm()) );
        CHECK( (v * k).Norm() == Approx(k * v.Norm()) );
    }

    SECTION("Triangle inequality")
    {
        auto u = GENERATE(take(100, RandomFiniteVec3(-10.0, 10.0)));
        auto v = GENERATE(take(100, RandomFiniteVec3(-10.0, 10.0)));
        CHECK( (u + v).Norm() <= u.Norm() + v.Norm() );
    }
}

TEST_CASE("SquaredNorm : Vec3 -> R", "[Vec3]")
{
    SECTION("It is the square of the norm")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3(-10.0, 10.0)));
        CHECK( v.SquaredNorm() == Approx(v.Norm() * v.Norm()) );
    }
}

TEST_CASE("Normalize : Vec3 -> S^3", "[Vec3]")
{
    SECTION("Vector is unitary after normalizing")
    {
        auto v = GENERATE(take(100, filter(CanVec3BeUsedToDivide,RandomFiniteVec3(10.0, 10.0))));
        v.Normalize();
        Vec3 v_normalized = v;
        CHECK(v_normalized.Norm() ==  Approx(1.0) );
    }

    SECTION("Normalize comp Normalize = Normalize")
    {
        auto v = GENERATE(take(100, filter(CanVec3BeUsedToDivide,RandomFiniteVec3(10.0, 10.0))));
        Vec3 v_normalized = v;
        v_normalized.Normalize();
        Vec3 v_norm_norm = v_normalized;
        v_norm_norm.Normalize();
        CHECK_THAT( v_norm_norm,
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, v_normalized) );
    }
}

TEST_CASE("Dot : Vec3 x Vec3 -> R", "[Vec3]")
{
    SECTION("Scalar product with (0, 0, 0) is 0")
    {
        Vec3 zero;
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( Dot(zero, v) == 0.0 );
        CHECK( Dot(v, zero) == 0.0 );
    }

    SECTION("Canonical scalar product has identity matrix in natural basis")
    {
        Vec3 e1(1.0, 0.0, 0.0);
        Vec3 e2(0.0, 1.0, 0.0);
        Vec3 e3(0.0, 0.0, 1.0);

        CHECK( Dot(e1, e1) == 1.0 );
        CHECK( Dot(e1, e2) == 0.0 );
        CHECK( Dot(e1, e3) == 0.0 );

        CHECK( Dot(e2, e1) == 0.0 );
        CHECK( Dot(e2, e2) == 1.0 );
        CHECK( Dot(e2, e3) == 0.0 );

        CHECK( Dot(e3, e1) == 0.0 );
        CHECK( Dot(e3, e2) == 0.0 );
        CHECK( Dot(e3, e3) == 1.0 );
    }

    SECTION("e_i capture i-th component")
    {
        Vec3 e1(1.0, 0.0, 0.0);
        Vec3 e2(0.0, 1.0, 0.0);
        Vec3 e3(0.0, 0.0, 1.0);
        auto v = GENERATE(take(100, RandomFiniteVec3()));

        CHECK( Dot(e1, v) == v[0] );
        CHECK( Dot(e2, v) == v[1] );
        CHECK( Dot(e3, v) == v[2] );

        CHECK( Dot(v, e1) == v[0] );
        CHECK( Dot(v, e2) == v[1] );
        CHECK( Dot(v, e3) == v[2] );
    }

    SECTION("Evaluated in the same vector, it results in its squared norm")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( Dot(v, v) == v.SquaredNorm() );
    }

    SECTION("Scalar factors can be extracted from scalar product")
    {
        auto u = GENERATE(take(10, RandomFiniteVec3(-10.0, 10.0)));
        auto v = GENERATE(take(10, RandomFiniteVec3(-10.0, 10.0)));
        auto k = GENERATE(take(10, random(Real(-10.0), Real(10.0))));

        CHECK( Dot(k*u, v) == Approx(k * Dot(u, v)).epsilon(tconst::kToleranceEqualityCheck) );
        CHECK( Dot(u, k*v) == Approx(k * Dot(u, v)).epsilon(tconst::kToleranceEqualityCheck) );
        CHECK( Dot(u*k, v) == Approx(k * Dot(u, v)).epsilon(tconst::kToleranceEqualityCheck) );
        CHECK( Dot(u, v*k) == Approx(k * Dot(u, v)).epsilon(tconst::kToleranceEqualityCheck) );
    }

    SECTION("Scalar product has distributive property with respect to +")
    {
        auto u = GENERATE(take(10, RandomFiniteVec3(-10.0, 10.0)));
        auto v = GENERATE(take(10, RandomFiniteVec3(-10.0, 10.0)));
        auto w = GENERATE(take(10, RandomFiniteVec3(-10.0, 10.0)));
        CHECK( Dot(u+v, w) == Approx(Dot(u, w) + Dot(v, w)).epsilon(tconst::kToleranceEqualityCheck) );
        CHECK( Dot(u, v+w) == Approx(Dot(u, v) + Dot(u, w)).epsilon(tconst::kToleranceEqualityCheck) );
    }

    SECTION("Dot product of orthogonal vectors is 0")
    {
        auto v = GENERATE(take(5, RandomFiniteVec3(30.0, 30.0)));
        CHECK ( Dot(v, Vec3( v.Y(), -v.X(),      0)) == 0.0 );
        CHECK ( Dot(v, Vec3(-v.Y(),  v.X(),      0)) == 0.0 );
        CHECK ( Dot(v, Vec3( v.Z(),      0, -v.X())) == 0.0 );
        CHECK ( Dot(v, Vec3(-v.Z(),      0,  v.X())) == 0.0 );
        CHECK ( Dot(v, Vec3(     0,  v.Z(), -v.Y())) == 0.0 );
        CHECK ( Dot(v, Vec3(     0, -v.Z(),  v.Y())) == 0.0 );

        // Another couple of orthogonal vectors to v
        // that don't need one component to be 0
        CHECK ( Dot(v, Vec3(v.Z() - v.Y(),
                            v.X() - v.Z(),
                            v.Y() - v.X())) == 0.0 );

    }

    SECTION("Dot product relates to cosine")
    {
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        // it seems that variables can't be used with GENERATE, it has to be literals
        auto angle_rad = GENERATE(take(100, random(-1.5707963, 1.5707963)));
        RealNum cosine = Real(std::cos(angle_rad));
        RealNum sine = Real(std::sin(angle_rad));
        SECTION("Vectors inside plane xy")
        {
            Vec3 unitary_v(v.X(), v.Y(), Real(0.0));
            unitary_v.Normalize();
            Vec3 rotated_v(cosine * unitary_v.X() - sine * unitary_v.Y(),
                           sine * unitary_v.X() + cosine *unitary_v.Y(),
                           Real(0.0));
            CHECK( Dot(unitary_v, rotated_v) == Approx(cosine) );
            CHECK( Dot(rotated_v, unitary_v) == Approx(cosine) );
        }

        SECTION("Vectors inside plane xz")
        {
            Vec3 unitary_v(v.X(), Real(0.0), v.Z());
            unitary_v.Normalize();
            Vec3 rotated_v(cosine * unitary_v.X() - sine * unitary_v.Z(),
                           Real(0.0),
                           sine * unitary_v.X() + cosine *unitary_v.Z());
            CHECK( Dot(unitary_v, rotated_v) == Approx(cosine) );
            CHECK( Dot(rotated_v, unitary_v) == Approx(cosine) );
        }

        SECTION("Vectors inside plane yz")
        {
            Vec3 unitary_v(Real(0.0), v.Y(), v.Z());
            unitary_v.Normalize();
            Vec3 rotated_v(Real(0.0),
                           cosine * unitary_v.Y() - sine * unitary_v.Z(),
                           sine * unitary_v.Y() + cosine *unitary_v.Z());
            CHECK( Dot(unitary_v, rotated_v) == Approx(cosine) );
            CHECK( Dot(rotated_v, unitary_v) == Approx(cosine) );
        }
    }
}

TEST_CASE("Cross : Vec3 x Vec3 -> Vec3", "[Vec3]")
{
    SECTION("Cross product with (0, 0, 0) is 0")
    {
        Vec3 zero;
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( Cross(zero, v) == zero );
        CHECK( Cross(v, zero) == zero );
    }

    SECTION("Cross product of a vector with itself is 0")
    {
        Vec3 zero;
        auto v = GENERATE(take(100, RandomFiniteVec3()));
        CHECK( Cross(v, v) == zero );
    }

    SECTION("Correctly defined over the natural basis")
    {
        Vec3 e1(1.0, 0.0, 0.0);
        Vec3 e2(0.0, 1.0, 0.0);
        Vec3 e3(0.0, 0.0, 1.0);
        Vec3 zero;

        CHECK( Cross(e1, e1) == zero );
        CHECK( Cross(e1, e2) == e3 );
        CHECK( Cross(e1, e3) == -e2 );

        CHECK( Cross(e2, e1) == -e3 );
        CHECK( Cross(e2, e2) == zero );
        CHECK( Cross(e2, e3) == e1 );

        CHECK( Cross(e3, e1) == e2 );
        CHECK( Cross(e3, e2) == -e1 );
        CHECK( Cross(e3, e3) == zero );
    }

    SECTION("It is anticommuntative")
    {
        auto u = GENERATE(take(10, RandomFiniteVec3(-10.0, 10.0)));
        auto v = GENERATE(take(10, RandomFiniteVec3(-10.0, 10.0)));
        CHECK( Cross(u, v) == -Cross(v, u) );
    }

    SECTION("Scalar factors can be extracted from cross product")
    {
        auto u = GENERATE(take(10, RandomFiniteVec3(-2.0, 2.0)));
        auto v = GENERATE(take(10, RandomFiniteVec3(-2.0, 2.0)));
        auto k = GENERATE(take(10, random(-2.0, 2.0)));
        CHECK_THAT( Cross(k*u, v),
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, k*Cross(u, v)) );
        CHECK_THAT( Cross(u*k, v),
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, k*Cross(u, v)) );
        CHECK_THAT( Cross(u, k*v),
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, k*Cross(u, v)) );
        CHECK_THAT( Cross(u, v*k),
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, k*Cross(u, v)) );
    }

    SECTION("Croos product has distributive property with respect to +")
    {
        auto u = GENERATE(take(10, RandomFiniteVec3(-10.0, 10.0)));
        auto v = GENERATE(take(10, RandomFiniteVec3(-10.0, 10.0)));
        auto w = GENERATE(take(10, RandomFiniteVec3(-10.0, 10.0)));
        CHECK_THAT( Cross(u+v, w),
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, Cross(u, w) + Cross(v, w)) );
        CHECK_THAT( Cross(u, v+w),
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, Cross(u, v) + Cross(u, w)) );
    }

    SECTION("Cross product satisfies Jacobi identity")
    {
        auto u = GENERATE(take(10, RandomFiniteVec3(-5.0, 5.0)));
        auto v = GENERATE(take(10, RandomFiniteVec3(-5.0, 5.0)));
        auto w = GENERATE(take(10, RandomFiniteVec3(-5.0, 5.0)));
        Vec3 zero;
        CHECK_THAT( Cross(u, Cross(v, w))
             + Cross(v, Cross(w, u))
             + Cross(w, Cross(u, v)),
             IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, zero) );
    }

    SECTION("Cross product relates to sine")
    {
        // This property is equivalent to "Norm of cross product
        // is area of paralellogram"
        auto v = GENERATE(take(100, RandomFiniteVec3(-10.0, 10.0)));
        // it seems that variables can't be used with GENERATE, it has to be literals
        // Also, we avoid values of the angle very close to 0, starting at approx 1 deg
        auto angle_rad = GENERATE(take(100, random(Real(0.02), Real(0.78539812))));
        RealNum cosine = Real(std::cos(angle_rad));
        RealNum sine = Real(std::sin(angle_rad));
        SECTION("Vectors inside plane xy")
        {
            Vec3 unitary_v(v.X(), v.Y(), Real(0.0));
            unitary_v.Normalize();
            Vec3 rotated_v(cosine * unitary_v.X() - sine * unitary_v.Y(),
                           sine * unitary_v.X() + cosine *unitary_v.Y(),
                           Real(0.0));
            CHECK( Cross(unitary_v, rotated_v).Norm() == Approx(sine) );
        }

        SECTION("Vectors inside plane xz")
        {
            Vec3 unitary_v(v.X(), Real(0.0), v.Z());
            unitary_v.Normalize();
            Vec3 rotated_v(cosine * unitary_v.X() - sine * unitary_v.Z(),
                           Real(0.0),
                           sine * unitary_v.X() + cosine *unitary_v.Z());
            CHECK( Cross(unitary_v, rotated_v).Norm() == Approx(sine) );
        }

        SECTION("Vectors inside plane yz")
        {
            Vec3 unitary_v(Real(0.0), v.Y(), v.Z());
            unitary_v.Normalize();
            Vec3 rotated_v(Real(0.0),
                           cosine * unitary_v.Y() - sine * unitary_v.Z(),
                           sine * unitary_v.Y() + cosine *unitary_v.Z());
            CHECK( Cross(unitary_v, rotated_v).Norm() == Approx(sine) );
        }
    }
}

TEST_CASE("UnitVector : Vec3 -> Vec3", "[Vec3]")
{
    SECTION("Resulting vector is unitary")
    {
        auto v = GENERATE(take(100, filter(CanVec3BeUsedToDivide,RandomFiniteVec3(10.0, 10.0))));
        CHECK( UnitVector(v).Norm() == Approx(1.0) );
    }
    SECTION("UnitVector of a normal vector is itself")
    {
        auto v = GENERATE(take(100, filter(CanVec3BeUsedToDivide,RandomFiniteVec3(10.0, 10.0))));
        v.Normalize();
        CHECK_THAT( UnitVector(v),
                    IsComponentWiseApprox<Vec3>(tconst::kToleranceEqualityCheck, v) );
    }
}

TEST_CASE("GetRandomPointInUnitBall", "[Vec3]")
{
    SECTION("Resulting Vec3 has norm <= 1")
    {
        // Seed random engine before each test
        my_engine(Catch::rngSeed());
        for(int i = 0; i < 100; ++i)
        {
            CHECK( GetRandomPointInUnitBall().Norm() <= 1.0 );
        }
    }
}

TEST_CASE("GetRandomPointInUnitDiscXY", "[Vec3]")
{
    SECTION("Resulting Vec3 has norm <= 1")
    {
        // Seed random engine before each test
        my_engine(Catch::rngSeed());
        for(int i = 0; i < 100; ++i)
        {
            CHECK( GetRandomPointInUnitDiscXY().Norm() <= 1.0 );
        }
    }
    SECTION("Resulting Vec3 has 3rd coordinate 0")
    {
        // Seed random engine before each test
        my_engine(Catch::rngSeed());
        for(int i = 0; i < 100; ++i)
        {
            CHECK( GetRandomPointInUnitDiscXY()[2] == 0.0 );
        }
    }
}

TEST_CASE("operator<< : Vec3 -> Beautiful text", "[Vec3]")
{
    std::ostringstream os;
    os << Vec3{1, 2, 3};
    CHECK( os.str() == "1 2 3");
    os.str(std::string{});
    os << Vec3{42.93, -2, 2.0};
    CHECK( os.str() == "42.93 -2 2");
}

TEST_CASE("operator>> : Pick a Vec3 -> Vec3", "[Vec3]")
{
    std::istringstream is;
    Vec3 v;
    SECTION("It reads correct numbers")
    {
        is.str("1e5 -2.005 9e-2");
        is >> v;
        CHECK( v == Vec3{1e5, -2.005, 9e-2} );
    }

    SECTION("It fails if it doesn't receive numbers")
    {
        is.str("a bc");
        v = Vec3{};
        CHECK( !(is >> v) );
        SECTION("and in that case it doesn't modify the vector")
        {
            CHECK( v == Vec3{} );
            SECTION("even if some values were valid")
            {
                v = Vec3{1, 2, 3};
                is.str("1e-1 2 not_a_number");
                CHECK(!(is >> v));
                CHECK(v == Vec3{1, 2, 3});
            }
        }
    }
}

} // namespace plemma::glancy

