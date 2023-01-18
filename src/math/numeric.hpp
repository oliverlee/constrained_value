#pragma once

#include "src/algebra.hpp"

#include <concepts>
#include <limits>

// clang-format off

namespace constrained_value::math {

// NOLINTBEGIN(misc-redundant-expression)

/// Specifies that a type fully implements `std::numeric_limits`
/// and models `std::totally_ordered` and is `std::default_initializable`.
///
template <typename T>
concept numeric_limits =
  std::totally_ordered<T> and
  std::default_initializable<T> and
  std::numeric_limits<T>::is_specialized and
  requires {
    std::same_as<bool, decltype(std::numeric_limits<T>::is_signed)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::is_integer)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::is_exact)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::has_infinity)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::has_quiet_NaN)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::has_signaling_NaN)>;
    std::same_as<std::float_denorm_style, decltype(std::numeric_limits<T>::has_denorm)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::has_denorm_loss)>;
    std::same_as<std::float_round_style, decltype(std::numeric_limits<T>::round_style)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::is_iec559)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::is_bounded)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::is_modulo)>;
    std::same_as<int, decltype(std::numeric_limits<T>::digits)>;
    std::same_as<int, decltype(std::numeric_limits<T>::digits10)>;
    std::same_as<int, decltype(std::numeric_limits<T>::max_digits10)>;
    std::same_as<int, decltype(std::numeric_limits<T>::radix)>;
    std::same_as<int, decltype(std::numeric_limits<T>::min_exponent)>;
    std::same_as<int, decltype(std::numeric_limits<T>::min_exponent10)>;
    std::same_as<int, decltype(std::numeric_limits<T>::max_exponent)>;
    std::same_as<int, decltype(std::numeric_limits<T>::max_exponent10)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::traps)>;
    std::same_as<bool, decltype(std::numeric_limits<T>::tinyness_before)>;
    { std::numeric_limits<T>::min() } -> std::same_as<T>;
    { std::numeric_limits<T>::lowest() } -> std::same_as<T>;
    { std::numeric_limits<T>::max() } -> std::same_as<T>;
    { std::numeric_limits<T>::epsilon() } -> std::same_as<T>;
    { std::numeric_limits<T>::round_error() } -> std::same_as<T>;
    { std::numeric_limits<T>::infinity() } -> std::same_as<T>;
    { std::numeric_limits<T>::quiet_NaN() } -> std::same_as<T>;
    { std::numeric_limits<T>::signaling_NaN() } -> std::same_as<T>;
    { std::numeric_limits<T>::denorm_min() } -> std::same_as<T>;
  } and
  noexcept(std::numeric_limits<T>::min()) and
  noexcept(std::numeric_limits<T>::lowest()) and
  noexcept(std::numeric_limits<T>::max()) and
  noexcept(std::numeric_limits<T>::epsilon()) and
  noexcept(std::numeric_limits<T>::round_error()) and
  noexcept(std::numeric_limits<T>::infinity()) and
  noexcept(std::numeric_limits<T>::quiet_NaN()) and
  noexcept(std::numeric_limits<T>::signaling_NaN()) and
  noexcept(std::numeric_limits<T>::denorm_min()) and
  (not std::numeric_limits<T>::is_bounded or
   ((std::numeric_limits<T>::lowest() <= T()) and
    (T() <= std::numeric_limits<T>::max()))) and
  (not std::numeric_limits<T>::is_bounded or
   ((std::numeric_limits<T>::is_integer) and (std::numeric_limits<T>::min() <= T())) or
   ((not std::numeric_limits<T>::is_integer) and (T() <= std::numeric_limits<T>::min()))) and
  (not std::numeric_limits<T>::has_infinity or
   (std::numeric_limits<T>::max() < std::numeric_limits<T>::infinity())) and
  (not std::numeric_limits<T>::has_quiet_NaN or
   (std::numeric_limits<T>::quiet_NaN() != std::numeric_limits<T>::quiet_NaN())) and
  (not std::numeric_limits<T>::has_signaling_NaN or
   (std::numeric_limits<T>::signaling_NaN() != std::numeric_limits<T>::signaling_NaN())) and
  (((std::numeric_limits<T>::has_denorm != std::denorm_absent) and
    (T() <= std::numeric_limits<T>::denorm_min()) and
    (std::numeric_limits<T>::denorm_min() <= std::numeric_limits<T>::min())) or
   ((std::numeric_limits<T>::has_denorm == std::denorm_absent) and
    (std::numeric_limits<T>::denorm_min() == T())));

// NOLINTEND(misc-redundant-expression)

/// Derive `numeric_limits<T>` from `std::numeric_limits<U>`
/// @{
template <typename T, numeric_limits U>
requires std::constructible_from<T, U>
struct derive_numeric_limits_from
{
  using limits = std::numeric_limits<U>;

  struct type : limits
  {
    static constexpr auto min() noexcept -> T { return T{limits::min()}; }
    static constexpr auto lowest() noexcept -> T { return T{limits::lowest()}; }
    static constexpr auto max() noexcept -> T { return T{limits::max()}; }
    static constexpr auto epsilon() noexcept -> T { return T{limits::epsilon()}; }
    static constexpr auto round_error() noexcept -> T { return T{limits::round_error()}; }
    static constexpr auto infinity() noexcept -> T { return T{limits::infinity()}; }
    static constexpr auto quiet_NaN() noexcept -> T { return T{limits::quiet_NaN()}; }
    static constexpr auto signaling_NaN() noexcept -> T { return T{limits::signaling_NaN()}; }
    static constexpr auto denorm_min() noexcept -> T { return T{limits::denorm_min()}; }
  };
};

template <typename T, typename U>
using derive_numeric_limits_from_t = typename derive_numeric_limits_from<T, U>::type;
/// @}

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
}

// clang-format on
