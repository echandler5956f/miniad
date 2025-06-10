// SPDX-License-Identifier: MIT
#pragma once
/// \file expr.hpp
/// \brief Base class for all expression template objects.

namespace miniad {

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

} // namespace miniad 