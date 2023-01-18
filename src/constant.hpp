#pragma once

#include "src/bitwise_integer.hpp"
#include "src/compare.hpp"
#include "src/math.hpp"
#include "src/ulp_distance.hpp"

#include <array>
#include <bit>
#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
#include <functional>
#include <limits>
#include <type_traits>

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace constrained_value {

/// Constants used with nttp_bindable predicates
///
/// Defines constants that are bindable to predicates adapted with
/// nttp_bindable. These constants are designed to be used as non-type template
/// parameters.
///
namespace constant {

/// Convertible zero constant
///
struct Zero
{
  /// Convert to the zero value of `T`
  /// @tparam T arbitrary type
  /// @requires `T{}` must be the zero value
  ///
  template <std::default_initializable T>
  [[nodiscard]] constexpr
  operator T() const noexcept(std::is_nothrow_default_constructible_v<T>)
  {
    return T{};
  }

  /// Defaulted comparisons
  /// @{
  [[nodiscard]] friend auto operator<=>(const Zero&, const Zero&) = default;
  [[nodiscard]] friend auto
  operator==(const Zero&, const Zero&) -> bool = default;
  /// @}

  // NOTE: explicit return types result in recursive template instantiation and
  // stack exhaustion with clang.
  // Comparability must be checked after determining T and Zero are different
  // types.

  /// Comparisons against arbitrary types
  /// @{
  template <std::default_initializable T>
    requires (not std::same_as<T, Zero> and std::equality_comparable<T>)
  [[nodiscard]] friend constexpr auto
  operator<=>(const T& x, const Zero&) noexcept(
      is_nothrow_partial_order_comparable_v<T>)
  {
    return x <=> T{};
  }

  template <std::default_initializable T>
    requires (not std::same_as<T, Zero> and std::three_way_comparable<T>)
  [[nodiscard]] friend constexpr auto operator==(
      const T& x, const Zero&) noexcept(is_nothrow_equality_comparable_v<T>)
  {
    return x == T{};
  }
  /// @}
};

/// ULP offset constant
///
/// Used to add or subtract ULP from a floating point value.
///
template <int N>
  requires (N != std::numeric_limits<int>::min())
struct ulp_offset
{
  static constexpr auto value = N;

  template <int M = N>
    requires (M == N)
  [[nodiscard]] friend consteval auto
  operator+(ulp_offset<M>) noexcept -> ulp_offset<M>
  {
    return {};
  }

  template <int M = N>
    requires (M == N)
  [[nodiscard]] friend consteval auto
  operator-(ulp_offset<M>) noexcept -> ulp_offset<-M>
  {
    return {};
  }

  template <
      math::iec559_floating_point T,
      typename L = std::numeric_limits<T>,
      auto offset = bitwise_integer_for_t<T>{N}>
  [[nodiscard]] friend consteval auto
  operator+(const T& x, ulp_offset) noexcept  // NOLINT(misc-no-recursion)
      -> T
  {
    if (math::signbit(x)) {
      // This can only recurse once and can only be invoked at compile time
      // NOLINTNEXTLINE(misc-no-recursion)
      return -(-x + -ulp_offset{});
    }

    assert(not math::signbit(x));
    assert(math::isfinite(x));

    if (const auto dist_to_zero = ulp_distance(x, T{}); offset < dist_to_zero) {
      const auto offset_past_zero = offset - dist_to_zero;
      const auto lowest_finite = bitwise_integer_reinterpretation(L::lowest());
      const auto minus_zero = bitwise_integer_reinterpretation(-T{});

      // -T{} reinterpreted as I is smaller than lowest_finite due to IEEE
      // representation
      assert(minus_zero < lowest_finite);

      // ensure result is finite:
      //   minus_zero - offset_past_zero <= lowest_finite
      assert(minus_zero - lowest_finite <= offset_past_zero);

      return std::bit_cast<T>(minus_zero - offset_past_zero);
    }

    const auto largest_finite = bitwise_integer_reinterpretation(L::max());

    // ensure result is finite:
    //   std::bit_cast<I>(x) + offset <= largest_finite
    assert(offset <= (largest_finite - bitwise_integer_reinterpretation(x)));

    return std::bit_cast<T>(bitwise_integer_reinterpretation(x) + offset);
  }

  template <typename T>
  [[nodiscard]] friend consteval auto
  operator-(const T& x, ulp_offset y) noexcept -> decltype(x + y)
  {
    return x + (-y);
  }
};

/// Variable template for an ULP offset
///
template <int N>
inline constexpr auto ulp = ulp_offset<N>{};

/// Wrapper for not yet supported non-type template arguments (e.g. double with
/// Clang)
template <typename T>
  requires (requires (T value) { std::bit_cast<T>(value); })
struct bitwise
{
  std::array<std::byte, sizeof(T)> data;

  consteval bitwise() noexcept(std::is_nothrow_default_constructible_v<T>)
    requires std::default_initializable<T>
      : bitwise(T{})
  {}

  consteval explicit bitwise(const T& value) noexcept
      : bitwise(std::bit_cast<bitwise>(value))
  {}

  [[nodiscard]] constexpr operator T() const noexcept
  {
    return std::bit_cast<T>(*this);
  }

  [[nodiscard]] constexpr auto value() const noexcept -> T
  {
    return static_cast<T>(*this);
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend consteval auto
  operator+(const bitwise<U>& x) noexcept(noexcept(+x.value()))
      -> decltype(bitwise<U>{+x.value()})
  {
    return bitwise<U>{+x.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend consteval auto operator-(const bitwise<U>& x) noexcept(
      std::is_nothrow_invocable_v<std::negate<>, U>)
      -> decltype(bitwise<U>{-x.value()})
  {
    return bitwise<U>{-x.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend consteval auto
  operator+(const bitwise<U>& x, const bitwise<U>& y) noexcept(
      std::is_nothrow_invocable_v<std::plus<>, U, U>)
      -> decltype(bitwise<U>{x.value() + y.value()})
  {
    return bitwise<U>{x.value() + y.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend consteval auto
  operator-(const bitwise<U>& x, const bitwise<U>& y) noexcept(
      std::is_nothrow_invocable_v<std::minus<>, U, U>)
      -> decltype(bitwise<U>{x.value() - y.value()})
  {
    return bitwise<U>{x.value() - y.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend consteval auto
  operator*(const bitwise<U>& x, const bitwise<U>& y) noexcept(
      std::is_nothrow_invocable_v<std::multiplies<>, U, U>)
      -> decltype(bitwise<U>{x.value() * y.value()})
  {
    return bitwise<U>{x.value() * y.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend consteval auto
  operator/(const bitwise<U>& x, const bitwise<U>& y) noexcept(
      std::is_nothrow_invocable_v<std::divides<>, U, U>)
      -> decltype(bitwise<U>{x.value() / y.value()})
  {
    return bitwise<U>{x.value() / y.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend constexpr auto
  operator<=>(const bitwise<U>& x, const bitwise<U>& y) noexcept(
      is_nothrow_partial_order_comparable_v<U>)
      -> decltype(x.value() <=> y.value())
  {
    return x.value() <=> y.value();
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend constexpr auto
  operator==(const bitwise<U>& x, const bitwise<U>& y) noexcept(
      is_nothrow_equality_comparable_v<U>) -> decltype(x.value() == y.value())
  {
    return x.value() == y.value();
  }
};

/// Function object for creating a `bitwise` constant
///
static constexpr struct
{
  [[nodiscard]] consteval auto operator()(auto value) const noexcept
  {
    return bitwise{value};
  }
} _{};

}  // namespace constant
}  // namespace constrained_value

/// Specialize `std::numeric_limits<bitwise<T>>` if `std::numeric_limits<T>` is
/// specialized
///
template <::constrained_value::math::numeric_limits T>
struct std::numeric_limits<::constrained_value::constant::bitwise<T>>
    : ::constrained_value::math::derive_numeric_limits_from_t<
          ::constrained_value::constant::bitwise<T>,
          T>
{};
