#pragma once

#include "src/algebra.hpp"

#include <bit>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace constrained_value {

namespace detail {

template <typename T>
consteval auto signed_integer_for() noexcept
{
  if constexpr (sizeof(T) == sizeof(std::int64_t)) {
    return std::int64_t{};
  } else if constexpr (sizeof(T) == sizeof(std::int32_t)) {
    return std::int32_t{};
  } else if constexpr (sizeof(T) == sizeof(std::int16_t)) {
    return std::int16_t{};
  } else if constexpr (sizeof(T) == sizeof(std::int8_t)) {
    return std::int8_t{};
  }
}

template <typename T>
using signed_integer_for_t = decltype(signed_integer_for<T>());

// NaN != NaN is `true`
// NOLINTBEGIN(misc-redundant-expression)

template <std::equality_comparable T>
[[nodiscard]] constexpr auto
isnan(T value) noexcept(noexcept(value != value)) -> bool
{
  return value != value;
}

// NOLINTEND(misc-redundant-expression)

// Redundant expressions are used to check `noexcept` of comparisons
// NOLINTBEGIN(misc-redundant-expression)

template <std::totally_ordered T>
  requires std::default_initializable<T>
[[nodiscard]] constexpr auto
abs(T value) noexcept(std::is_nothrow_default_constructible_v<T>and noexcept(
    value == value) and noexcept(value < value) and noexcept(-value)) -> T
{
  if (value == T{}) {
    // return +0.0 instead of -0.0
    return T{};
  }

  return (T{} < value) ? value : -value;
}

// NOLINTEND(misc-redundant-expression)

template <typename T, typename I = signed_integer_for_t<T>>
[[nodiscard]] constexpr auto signbit(T value) noexcept -> bool
{
  return std::bit_cast<I>(value) < I{};
}

template <std::signed_integral I, std::totally_ordered T>
[[nodiscard]] constexpr auto
signum(T x, T y) noexcept(noexcept(x < y) and noexcept(static_cast<I>(x))) -> I
{
  return static_cast<I>(y < x) - static_cast<I>(x < y);
}

}  // namespace detail

/// ULP/bit distance between two "floating point"-like values
///
template <
    algebra::additive_group T,
    std::same_as<T> U,
    std::signed_integral I = detail::signed_integer_for_t<T>>
  requires (std::numeric_limits<T>::is_iec559 and std::totally_ordered<T> and
            requires (T value) { std::bit_cast<I>(value); })
[[nodiscard]] constexpr auto ulp_distance(T a, U b) noexcept(
    noexcept(detail::isnan(a)) and noexcept(detail::abs(a)) and noexcept(
        detail::signum<I>(b, a)))
{
  using L = std::numeric_limits<T>;

  // TODO move this into a concept and predicate
  assert(a != +L::infinity());
  assert(a != -L::infinity());

  assert(b != +L::infinity());
  assert(b != -L::infinity());

  assert(not detail::isnan(a));
  assert(not detail::isnan(b));

  const auto x = std::bit_cast<I>(detail::abs(a));
  const auto y = std::bit_cast<I>(detail::abs(b));

  const auto dist =
      (detail::signbit(a) == detail::signbit(b)) ? detail::abs(x - y) : (x + y);

  return detail::signum<I>(b, a) * dist;
}

}  // namespace constrained_value
