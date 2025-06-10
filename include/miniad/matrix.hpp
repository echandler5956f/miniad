// SPDX-License-Identifier: MIT
#pragma once
/// \file matrix.hpp
/// \brief Statically-sized matrix class.

#include <array>
#include <miniad/scalar.hpp>
#include <miniad/expr.hpp>

namespace miniad {

/// \brief A statically-sized matrix of dual numbers.
/// \tparam T The underlying arithmetic type (e.g., float, double).
/// \tparam Rows The number of rows.
/// \tparam Cols The number of columns.
///
/// This class is the primary user-facing type for linear algebra operations.
/// It stores elements in a flat std::array for cache efficiency and is itself
/// an expression template node, allowing it to be seamlessly used in chains
/// of operations.
template <typename T, size_t Rows, size_t Cols>
class matrix : public expr_base<matrix<T, Rows, Cols>> {
public:
    // --- Public Type and Static Members ---
    using value_type = T;
    static constexpr size_t rows = Rows;
    static constexpr size_t cols = Cols;

    // --- Element Access ---

    /// \brief Provides mutable access to an element at (r, c).
    [[nodiscard]] constexpr scalar<T>& operator()(size_t r, size_t c) noexcept {
        return data_[r * cols + c];
    }

    /// \brief Provides immutable access to an element at (r, c).
    [[nodiscard]] constexpr const scalar<T>& operator()(size_t r, size_t c) const noexcept {
        return data_[r * cols + c];
    }

    // --- Expression Template Requirement ---

    /// \brief The evaluation of a matrix terminal node is just itself.
    [[nodiscard]] constexpr const matrix& eval() const noexcept {
        return *this;
    }

private:
    std::array<scalar<T>, rows * cols> data_{};
};

} // namespace miniad 