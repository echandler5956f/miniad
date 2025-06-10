#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <miniad/scalar.hpp>
#include <cmath>

using miniad::scalar;

TEST_CASE("scalar dual arithmetic", "[scalar][forward]") {
    scalar<double> x{3.0};
    scalar<double> y{4.0};

    SECTION("Value operations") {
        REQUIRE((x + y).value() == Catch::Approx(7.0));
        REQUIRE((x - y).value() == Catch::Approx(-1.0));
        REQUIRE((x * y).value() == Catch::Approx(12.0));
        REQUIRE((x / y).value() == Catch::Approx(0.75));
    }

    SECTION("Derivative propagation") {
        // For forward mode dual numbers, derivative of x w.r.t x is 1, of y is 0.
        x.set_derivative(1.0); // dx/dx = 1
        y.set_derivative(0.0); // dy/dx = 0

        REQUIRE((x + y).dual() == Catch::Approx(1.0));
        REQUIRE((x - y).dual() == Catch::Approx(1.0));
        REQUIRE((x * y).dual() == Catch::Approx(4.0)); // d(x*y)/dx = y = 4
        REQUIRE((x / y).dual() == Catch::Approx(1.0 / 4.0)); // d(x/y)/dx = 1/y
    }

    SECTION("Math functions and unary negation") {
        scalar<double> a{4.0, 1.0}; // value=4, da/dx=1

        auto neg_a = -a;
        REQUIRE(neg_a.value() == Catch::Approx(-4.0));
        REQUIRE(neg_a.dual() == Catch::Approx(-1.0));

        auto sqrt_a = sqrt(a); // sqrt(4) = 2
        // d(sqrt(a))/dx = (1 / (2*sqrt(a))) * da/dx = (1 / (2*2)) * 1 = 0.25
        REQUIRE(sqrt_a.value() == Catch::Approx(2.0));
        REQUIRE(sqrt_a.dual() == Catch::Approx(0.25));

        auto exp_a = exp(a); // exp(4)
        // d(exp(a))/dx = exp(a) * da/dx = exp(4) * 1
        REQUIRE(exp_a.value() == Catch::Approx(std::exp(4.0)));
        REQUIRE(exp_a.dual() == Catch::Approx(std::exp(4.0)));
    }

    SECTION("Edge cases") {
        scalar<double> zero{0.0, 1.0}; // value=0, d_zero/dx = 1

        // Test sqrt(0)
        auto sqrt_zero = sqrt(zero);
        REQUIRE(sqrt_zero.value() == Catch::Approx(0.0));
        REQUIRE(std::isinf(sqrt_zero.dual()));
    }
} 