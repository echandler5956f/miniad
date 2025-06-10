#include <catch2/catch_test_macros.hpp>
#include <miniad/scalar.hpp>

using miniad::scalar;

template<typename T>
constexpr auto approx(T value) {
    return Catch::Approx(static_cast<double>(value));
}

TEST_CASE("scalar dual arithmetic", "[scalar][forward]") {
    scalar<double> x{3.0};
    scalar<double> y{4.0};

    SECTION("Value operations") {
        REQUIRE((x + y).value() == approx(7.0));
        REQUIRE((x - y).value() == approx(-1.0));
        REQUIRE((x * y).value() == approx(12.0));
        REQUIRE((x / y).value() == approx(0.75));
    }

    SECTION("Derivative propagation") {
        // For forward mode dual numbers, derivative of x w.r.t x is 1, of y is 0.
        x.set_derivative(1.0); // dx/dx = 1
        y.set_derivative(0.0); // dy/dx = 0

        REQUIRE((x + y).dual() == approx(1.0));
        REQUIRE((x - y).dual() == approx(1.0));
        REQUIRE((x * y).dual() == approx(4.0)); // d(x*y)/dx = y = 4
        REQUIRE((x / y).dual() == approx(1.0 / 4.0)); // d(x/y)/dx = 1/y
    }
} 