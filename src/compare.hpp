#pragma once

#include <utility>

namespace constrained_value {
// clang-format off
// NOLINTBEGIN(misc-redundant-expression)

template <typename T>
inline constexpr auto is_nothrow_equality_comparable_v =
  noexcept(std::declval<const T&>() == std::declval<const T&>()) and
  noexcept(std::declval<const T&>() != std::declval<const T&>());

template <typename T>
inline constexpr auto is_nothrow_partial_order_comparable_v =
  noexcept(std::declval<const T&>() <  std::declval<const T&>()) and
  noexcept(std::declval<const T&>() >  std::declval<const T&>()) and
  noexcept(std::declval<const T&>() <= std::declval<const T&>()) and
  noexcept(std::declval<const T&>() >= std::declval<const T&>());

template <typename T>
inline constexpr auto is_nothrow_total_order_comparable_v =
  is_nothrow_partial_order_comparable_v<T> and
  is_nothrow_equality_comparable_v<T>;

// NOLINTEND(misc-redundant-expression)
// clang-format on
}  // namespace constrained_value
