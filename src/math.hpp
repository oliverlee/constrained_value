#pragma once

#include "src/algebra.hpp"
#include "src/bitwise_integer.hpp"
#include "src/compare.hpp"

#include <concepts>
#include <functional>
#include <limits>
#include <type_traits>

// numeric = numeric_limits + additive_group + totally_ordered

// clang-format off

namespace constrained_value::math {

// NOLINTBEGIN(misc-redundant-expression)

/// Specifies that a type fully implements `std::numeric_limits`
/// and models `std::totally_ordered` and is `std::default_initializable`.
///
template <typename T, typename L = std::numeric_limits<T>>
concept numeric_limits =
  std::same_as<L, std::numeric_limits<T>> and
  std::totally_ordered<T> and
  std::default_initializable<T> and
  L::is_specialized and
  requires {
    std::same_as<bool, decltype(L::is_signed)>;
    std::same_as<bool, decltype(L::is_integer)>;
    std::same_as<bool, decltype(L::is_exact)>;
    std::same_as<bool, decltype(L::has_infinity)>;
    std::same_as<bool, decltype(L::has_quiet_NaN)>;
    std::same_as<bool, decltype(L::has_signaling_NaN)>;
    std::same_as<std::float_denorm_style, decltype(L::has_denorm)>;
    std::same_as<bool, decltype(L::has_denorm_loss)>;
    std::same_as<std::float_round_style, decltype(L::round_style)>;
    std::same_as<bool, decltype(L::is_iec559)>;
    std::same_as<bool, decltype(L::is_bounded)>;
    std::same_as<bool, decltype(L::is_modulo)>;
    std::same_as<int, decltype(L::digits)>;
    std::same_as<int, decltype(L::digits10)>;
    std::same_as<int, decltype(L::max_digits10)>;
    std::same_as<int, decltype(L::radix)>;
    std::same_as<int, decltype(L::min_exponent)>;
    std::same_as<int, decltype(L::min_exponent10)>;
    std::same_as<int, decltype(L::max_exponent)>;
    std::same_as<int, decltype(L::max_exponent10)>;
    std::same_as<bool, decltype(L::traps)>;
    std::same_as<bool, decltype(L::tinyness_before)>;
    { L::min() } -> std::same_as<T>;
    { L::lowest() } -> std::same_as<T>;
    { L::max() } -> std::same_as<T>;
    { L::epsilon() } -> std::same_as<T>;
    { L::round_error() } -> std::same_as<T>;
    { L::infinity() } -> std::same_as<T>;
    { L::quiet_NaN() } -> std::same_as<T>;
    { L::signaling_NaN() } -> std::same_as<T>;
    { L::denorm_min() } -> std::same_as<T>;
  } and
  noexcept(L::min()) and
  noexcept(L::lowest()) and
  noexcept(L::max()) and
  noexcept(L::epsilon()) and
  noexcept(L::round_error()) and
  noexcept(L::infinity()) and
  noexcept(L::quiet_NaN()) and
  noexcept(L::signaling_NaN()) and
  noexcept(L::denorm_min()) and
  (not L::is_bounded or
   ((L::lowest() <= T()) and
    (T() <= L::max()))) and
  (not L::is_bounded or
   ((L::is_integer) and (L::min() <= T())) or
   ((not L::is_integer) and (T() <= L::min()))) and
  (not L::has_infinity or (L::max() < L::infinity())) and
  (not L::has_quiet_NaN or (L::quiet_NaN() != L::quiet_NaN())) and
  (not L::has_signaling_NaN or (L::signaling_NaN() != L::signaling_NaN())) and
  (((L::has_denorm != std::denorm_absent) and
    (T() <= L::denorm_min()) and
    (L::denorm_min() <= L::min())) or
   ((L::has_denorm == std::denorm_absent) and
    (L::denorm_min() == T())));

// NOLINTEND(misc-redundant-expression)

/// Specifies that a type fully implements `std::numeric_limits` and models
/// `algebra::additive_group` and `std::totally_ordered`.
///
template <typename T>
concept numeric =
  numeric_limits<T> and
  algebra::additive_group<T> and
  std::totally_ordered<T>;

/// Specifies that a type fulfills the requirements of the IEC 559 (IEEE 754)
/// standard.
///
template <typename T>
concept iec559_floating_point =
  numeric<T> and
  std::numeric_limits<T>::is_iec559 and
  std::numeric_limits<T>::has_infinity and
  std::numeric_limits<T>::has_quiet_NaN and
  std::numeric_limits<T>::has_signaling_NaN;

// TODO add this as a case to `projection::abs`
inline constexpr struct
{
  template <std::totally_ordered T>
  requires std::default_initializable<T>
  [[nodiscard]]
  constexpr auto operator()(const T& value) const
      noexcept(std::is_nothrow_default_constructible_v<T> and
               is_nothrow_total_order_comparable_v<T> and
               std::is_nothrow_invocable_v<std::negate<>, const T&>)
    -> T
  {
    if (value == T{}) {
      // return +0.0 instead of -0.0
      return T{};
    }

    return (T{} < value) ? value : -value;
  }
} abs{};

inline constexpr struct
{
  template <std::equality_comparable T>
  [[nodiscard]]
  constexpr auto operator()(const T& value) const
      noexcept(is_nothrow_equality_comparable_v<T>)
    -> bool
  {
    // NaN != NaN is `true`
    // NOLINTNEXTLINE(misc-redundant-expression)
    return value != value;
  }
} isnan{};

inline constexpr struct
{
  template <numeric T, typename L = std::numeric_limits<T>>
  [[nodiscard]]
  constexpr auto operator()(const T& value) const
    noexcept(is_nothrow_equality_comparable_v<T> and
             std::is_nothrow_invocable_v<std::negate<>, const T&>)
    -> bool
  {
    return (value == L::infinity()) or
           (value == -L::infinity());
  }
} isinfinite{};

inline constexpr struct
{
  template <numeric T>
  [[nodiscard]]
  constexpr auto operator()(const T& value) const
    noexcept(std::is_nothrow_invocable_v<decltype(isinfinite), const T&>)
    -> bool
  {
    return not(isnan(value) or isinfinite(value));
  }
} isfinite{};

inline constexpr struct
{
  template <bitwise_integer_reinterpretable T>
  [[nodiscard]]
  constexpr auto operator()(const T& value) const noexcept
    -> bool
  {
    return bitwise_integer_reinterpretation(value) < bitwise_integer_for_t<T>{};
  }
} signbit{};

namespace detail {
template <std::signed_integral I>
struct signum_fn
{
  template <std::totally_ordered T>
  [[nodiscard]]
  constexpr auto operator()(const T& x, const T& y) const
    noexcept(is_nothrow_partial_order_comparable_v<T>)
    -> I
  {
    return static_cast<I>(y < x) - static_cast<I>(x < y);
  }
};
}  // namespace detail
template <std::signed_integral I>
inline constexpr auto signum = detail::signum_fn<I>{};

} // namespace constrained_value::math

// clang-format on
