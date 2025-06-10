// SPDX-License-Identifier: MIT
#pragma once
/// \file scalar.hpp
/// \brief Dual number scalar<T> with forward-mode automatic differentiation.

#include <miniad/expr.hpp>
#include <type_traits>
#include <cmath>

namespace miniad {

/// \brief Dual number representing a scalar value and its first derivative.
///
/// scalar<T> is now a terminal node in an expression tree. It inherits
/// from expr_base so it can be used in generic expression template operations.
template <typename T>
class scalar : public expr_base<scalar<T>> {
    static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>,
                  "scalar<T> requires arithmetic T");

public:
    using value_type = T;

    // Constructors
    constexpr scalar() noexcept(std::is_nothrow_default_constructible_v<T>)
        : value_(), deriv_() {}

    constexpr explicit scalar(T value, T derivative = T{0}) noexcept
        : value_{value}, deriv_{derivative} {}

    // --- Assignment ---
    constexpr scalar& operator=(const T& value) noexcept {
        value_ = value;
        deriv_ = T{0};
        return *this;
    }

    // Accessors
    [[nodiscard]] constexpr T value() const noexcept { return value_; }
    [[nodiscard]] constexpr T dual() const noexcept { return deriv_; }
    constexpr void set_derivative(T d) noexcept { deriv_ = d; }

    /// \brief The evaluation of a scalar terminal is just itself.
    [[nodiscard]] constexpr const scalar& eval() const noexcept { return *this; }

private:
    T value_{};
    T deriv_{};
};

// All arithmetic operators are now generic and live in expr.hpp

} // namespace miniad