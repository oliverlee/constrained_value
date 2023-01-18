#pragma once

#include "src/algebra.hpp"
#include "src/bitwise_integer.hpp"
#include "src/compare.hpp"
#include "src/math/numeric.hpp"

#include <concepts>
#include <functional>
#include <type_traits>

namespace constrained_value::math {

/// Computes the absolute value of a totally ordered number
/// @see https://en.cppreference.com/w/cpp/numeric/math/fabs
///
// TODO add this as a case to `projection::abs`
inline constexpr struct
{
  template <std::totally_ordered T>
    requires std::default_initializable<T>
  [[nodiscard]] constexpr auto operator()(const T& value) const noexcept(
      std::is_nothrow_default_constructible_v<
          T>and is_nothrow_total_order_comparable_v<T>and
          std::is_nothrow_invocable_v<std::negate<>, const T&>) -> T
  {
    if (value == T{}) {
      // return +0.0 instead of -0.0
      return T{};
    }

    return (T{} < value) ? value : -value;
  }
} abs{};

/// Determines if a value is non-a-number
/// @see https://en.cppreference.com/w/cpp/numeric/math/isnan
///
inline constexpr struct
{
  template <std::equality_comparable T>
  [[nodiscard]] constexpr auto operator()(const T& value) const
      noexcept(is_nothrow_equality_comparable_v<T>) -> bool
  {
    // NaN != NaN is `true`
    // NOLINTNEXTLINE(misc-redundant-expression)
    return value != value;
  }
} isnan{};

/// Determines if a number if has a infinite value
/// @see https://en.cppreference.com/w/cpp/numeric/math/isinf
///
inline constexpr struct
{
  template <numeric T, typename L = std::numeric_limits<T>>
  [[nodiscard]] constexpr auto operator()(const T& value) const noexcept(
      is_nothrow_equality_comparable_v<T>and
          std::is_nothrow_invocable_v<std::negate<>, const T&>) -> bool
  {
    return (value == L::infinity()) or (value == -L::infinity());
  }
} isinf{};

/// Determines if a number if has a finite value
/// @see https://en.cppreference.com/w/cpp/numeric/math/isfinite
///
inline constexpr struct
{
  template <numeric T>
  [[nodiscard]] constexpr auto operator()(const T& value) const
      noexcept(std::is_nothrow_invocable_v<decltype(isinf), const T&>) -> bool
  {
    return not(isnan(value) or isinf(value));
  }
} isfinite{};

/// Determines if a number is negative
/// @see https://en.cppreference.com/w/cpp/numeric/math/signbit
///
inline constexpr struct
{
  template <bitwise_integer_reinterpretable T>
  [[nodiscard]] constexpr auto operator()(const T& value) const noexcept -> bool
  {
    return bitwise_integer_reinterpretation(value) < bitwise_integer_for_t<T>{};
  }
} signbit{};

/// Returns the sign of a number as an integer
/// @see https://en.cppreference.com/w/cpp/numeric/math/signbit
///
namespace detail {
template <std::signed_integral I>
struct signum_fn
{
  // TODO use partial_ordering
  // https://stackoverflow.com/questions/61604694/check-if-a-type-is-a-partial-order
  template <std::totally_ordered T>
  [[nodiscard]] constexpr auto operator()(const T& x, const T& y) const
      noexcept(is_nothrow_partial_order_comparable_v<T>) -> I
  {
    return static_cast<I>(y < x) - static_cast<I>(x < y);
  }
};
}  // namespace detail
template <std::signed_integral I>
inline constexpr auto signum = detail::signum_fn<I>{};

}  // namespace constrained_value::math
