#include <catch2/catch_test_macros.hpp>
#include <miniad/matrix.hpp>
#include <type_traits>

TEST_CASE("Matrix Structure and Access", "[matrix]") {
    using namespace miniad;

    // Define a 2x3 matrix of doubles.
    matrix<double, 2, 3> A;

    // Test 1: Compile-time properties.
    // We use static_assert because these are properties of the type itself,
    // not of a specific object. They should be true at compile time.
    static_assert(A.rows == 2, "Matrix must have a static rows member.");
    static_assert(A.cols == 3, "Matrix must have a static cols member.");
    static_assert(std::is_base_of_v<expr_base<matrix<double, 2, 3>>, matrix<double, 2, 3>>,
                  "Matrix must be an expression template.");

    // Test 2: Element access.
    // We check that we can write to an element and then read it back.
    SECTION("Element read/write access") {
        A(0, 2) = 42.0;
        REQUIRE(A(0, 2).value() == 42.0);

        A(1, 1) = -7.5;
        REQUIRE(A(1, 1).value() == -7.5);
    }

    // Test 3: Constexpr access.
    // This is a powerful feature. We create a constexpr function that
    // initializes and inspects a matrix at COMPILE TIME. If this compiles,
    // our constexpr correctness test passes.
    SECTION("Constexpr element access") {
        constexpr bool can_use_in_constexpr = [] {
            matrix<float, 2, 2> M;
            M(0, 0) = 1.0f;
            M(1, 1) = M(0, 0);
            return M(1, 1).value() == 1.0f;
        }();
        REQUIRE(can_use_in_constexpr);
    }
} 