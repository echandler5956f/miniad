#include <catch2/catch_test_macros.hpp>
#include <miniad/expr.hpp>
#include <miniad/scalar.hpp>
#include <type_traits>

// A dummy expression type for testing the base class.
// It inherits from expr_base using CRTP.
template<typename T>
struct dummy_expr : miniad::expr_base<dummy_expr<T>> {
    // This expression always evaluates to a constant value.
    constexpr T eval() const { return T{42}; }
};

TEST_CASE("Expression base class", "[expr]") {
    dummy_expr<double> expr;

    // Test 1: The base class must provide a .self() method
    // to correctly cast down to the derived type.
    const miniad::expr_base<dummy_expr<double>>& base_ref = expr;
    const auto& derived_ref = base_ref.self();
    REQUIRE(std::is_same_v<decltype(derived_ref), const dummy_expr<double>&>);

    // Test 2: The .self().eval() chain should work and return the correct value.
    REQUIRE(derived_ref.eval() == 42.0);

    // Test 3: The base class must provide an implicit conversion operator
    // that calls eval() on the derived object for convenience.
    double value = expr;
    REQUIRE(value == 42.0);
}

TEST_CASE("Expression Composition", "[expr]") {
    using namespace miniad;
    scalar<double> x{2.0};
    scalar<double> y{3.0};

    auto add_expression = x + y;

    // Test 1: The result of an operation should be an expression object,
    // NOT the fully computed scalar value.
    REQUIRE(!std::is_same_v<decltype(add_expression), scalar<double>>);

    // Test 2: The resulting expression object must inherit from expr_base
    // to be part of the expression template system.
    REQUIRE(std::is_base_of_v<expr_base<decltype(add_expression)>, decltype(add_expression)>);
} 