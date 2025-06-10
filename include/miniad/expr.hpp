// SPDX-License-Identifier: MIT
#pragma once
/// \file expr.hpp
/// \brief Base class and operators for expression templates.

#include <utility> // for std::forward

namespace miniad {

// Forward-declare scalar so operators can return it.
template<typename T> class scalar;

/// \brief Base class for expression template nodes using CRTP.
/// \tparam E The derived expression type (e.g., add_expr<...>)
///
/// This class provides the common interface for all expression objects.
/// The core idea is that operations like `+` don't compute a result
/// immediately. Instead, they return a lightweight expression object that
/// represents the computation. The actual computation is deferred until the
/// expression is assigned to a variable, at which point `eval()` is called.
template <typename E>
class expr_base {
public:
    /// \brief Returns a reference to the derived expression object.
    /// This is the "downcast" part of CRTP.
    [[nodiscard]] constexpr E& self() noexcept { return static_cast<E&>(*this); }

    /// \brief Returns a const reference to the derived expression object.
    [[nodiscard]] constexpr const E& self() const noexcept {
        return static_cast<const E&>(*this);
    }

    /// \brief Implicit conversion operator.
    ///
    /// Evaluates the expression by calling `eval()` on the derived type.
    /// This allows writing `double result = my_expr;`
    [[nodiscard]] constexpr operator auto() const { return self().eval(); }

protected:
    // Protected constructor and destructor to ensure it's only used as a base.
    // Defaulted functions are trivial and allow for constexpr.
    constexpr expr_base() = default;
    ~expr_base() = default;
    expr_base(const expr_base&) = default;
    expr_base(expr_base&&) = default;
    expr_base& operator=(const expr_base&) = default;
    expr_base& operator=(expr_base&&) = default;
};


// --- Operator Policy Structs ---
// These structs define the `apply` logic for values and derivatives.

struct add_op {
    template <typename T>
    static constexpr auto apply(const T& a, const T& b) noexcept {
        return scalar<typename T::value_type>{a.value() + b.value(), a.dual() + b.dual()};
    }
};

struct sub_op {
    template <typename T>
    static constexpr auto apply(const T& a, const T& b) noexcept {
        return scalar<typename T::value_type>{a.value() - b.value(), a.dual() - b.dual()};
    }
};

struct mul_op {
    template <typename T>
    static constexpr auto apply(const T& a, const T& b) noexcept {
        return scalar<typename T::value_type>{
            a.value() * b.value(),
            a.dual() * b.value() + a.value() * b.dual()
        };
    }
};

struct div_op {
    template <typename T>
    static constexpr auto apply(const T& a, const T& b) noexcept {
        using V = typename T::value_type;
        const auto inv_b_val = V{1} / b.value();
        return scalar<V>{
            a.value() * inv_b_val,
            (a.dual() * b.value() - a.value() * b.dual()) * inv_b_val * inv_b_val
        };
    }
};


// --- Binary Expression Template ---

template <typename Op, typename E1, typename E2>
class binary_expr : public expr_base<binary_expr<Op, E1, E2>> {
public:
    using value_type = typename E1::value_type;

    constexpr binary_expr(const E1& lhs, const E2& rhs) noexcept
        : lhs_(lhs), rhs_(rhs) {}

    [[nodiscard]] constexpr auto eval() const noexcept {
        // Recurse down the expression tree, then apply the operation.
        return Op::apply(lhs_.eval(), rhs_.eval());
    }

private:
    const E1& lhs_;
    const E2& rhs_;
};


// --- Operator Overloads ---
// These functions DO NOT COMPUTE. They return a lightweight binary_expr object.

template <typename E1, typename E2>
[[nodiscard]] constexpr auto operator+(const expr_base<E1>& a, const expr_base<E2>& b) noexcept {
    return binary_expr<add_op, E1, E2>(a.self(), b.self());
}

template <typename E1, typename E2>
[[nodiscard]] constexpr auto operator-(const expr_base<E1>& a, const expr_base<E2>& b) noexcept {
    return binary_expr<sub_op, E1, E2>(a.self(), b.self());
}

template <typename E1, typename E2>
[[nodiscard]] constexpr auto operator*(const expr_base<E1>& a, const expr_base<E2>& b) noexcept {
    return binary_expr<mul_op, E1, E2>(a.self(), b.self());
}

template <typename E1, typename E2>
[[nodiscard]] constexpr auto operator/(const expr_base<E1>& a, const expr_base<E2>& b) noexcept {
    return binary_expr<div_op, E1, E2>(a.self(), b.self());
}

} // namespace miniad 