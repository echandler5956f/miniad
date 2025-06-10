// SPDX-License-Identifier: MIT
#pragma once
/// \file scalar.hpp
/// \brief Dual number scalar<T> with forward-mode automatic differentiation.

#include <type_traits>

namespace miniad {

/// \brief Dual number representing a scalar value and its first derivative.
/// \tparam T underlying arithmetic type (float, double, etc.)
///
/// Design notes:
/// 1. All operations are `constexpr` to enable compile-time evaluation where possible.
/// 2. All trivial accessors and math operations are marked `noexcept` when they cannot throw.
/// 3. Implementation follows forward-mode AD rules: `(f * g)' = f' * g + f * g'` etc.
/// 4. This class is a thin POD-like wrapper; size is two scalars.
/// 5. Not intended for polymorphic use – no virtual functions.
template <typename T>
class scalar {
    static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>,
                  "scalar<T> requires arithmetic T");

public:
    using value_type = T;

    // Constructors ----------------------------------------------------------
    constexpr scalar() noexcept(std::is_nothrow_default_constructible_v<T>)
        : value_(), deriv_() {}

    constexpr explicit scalar(T value, T derivative = T{0}) noexcept
        : value_{value}, deriv_{derivative} {}

    // Accessors -------------------------------------------------------------
    [[nodiscard]] constexpr T value() const noexcept { return value_;
    }

    [[nodiscard]] constexpr T dual() const noexcept { return deriv_; }

    constexpr void set_derivative(T d) noexcept { deriv_ = d; }

    // Arithmetic operators --------------------------------------------------
    friend constexpr scalar operator+(const scalar& lhs, const scalar& rhs) noexcept {
        return scalar{lhs.value_ + rhs.value_, lhs.deriv_ + rhs.deriv_};
    }
    friend constexpr scalar operator-(const scalar& lhs, const scalar& rhs) noexcept {
        return scalar{lhs.value_ - rhs.value_, lhs.deriv_ - rhs.deriv_};
    }
    friend constexpr scalar operator*(const scalar& lhs, const scalar& rhs) noexcept {
        return scalar{lhs.value_ * rhs.value_, lhs.deriv_ * rhs.value_ + lhs.value_ * rhs.deriv_};
    }
    friend constexpr scalar operator/(const scalar& lhs, const scalar& rhs) noexcept {
        const T inv = T{1} / rhs.value_;
        return scalar{lhs.value_ * inv,
                      (lhs.deriv_ * rhs.value_ - lhs.value_ * rhs.deriv_) * inv * inv};
    }

    // Compound-assignment ----------------------------------------------------
    constexpr scalar& operator+=(const scalar& other) noexcept {
        value_ += other.value_;
        deriv_ += other.deriv_;
        return *this;
    }
    constexpr scalar& operator-=(const scalar& other) noexcept {
        value_ -= other.value_;
        deriv_ -= other.deriv_;
        return *this;
    }
    constexpr scalar& operator*=(const scalar& other) noexcept {
        deriv_ = deriv_ * other.value_ + value_ * other.deriv_;
        value_ *= other.value_;
        return *this;
    }
    constexpr scalar& operator/=(const scalar& other) noexcept {
        const T inv = T{1} / other.value_;
        deriv_ = (deriv_ * other.value_ - value_ * other.deriv_) * inv * inv;
        value_ *= inv;
        return *this;
    }

private:
    T value_{};
    T deriv_{};
};

} // namespace miniad 