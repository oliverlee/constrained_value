#pragma once

#include <array>
#include <bit>
#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
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
  operator<=>(const T& x, const Zero&) noexcept(noexcept(x <=> T{}))
  {
    return x <=> T{};
  }

  template <std::default_initializable T>
    requires (not std::same_as<T, Zero> and std::three_way_comparable<T>)
  [[nodiscard]] friend constexpr auto
  operator==(const T& x, const Zero&) noexcept(noexcept(x == T{}))
  {
    return x == T{};
  }
  /// @}
};

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
  {
    assert(value == value);
  }

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
  operator-(const bitwise<U>& x) noexcept(noexcept(-x.value())) -> bitwise<U>
  {
    return bitwise<U>{-x.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend consteval auto
  operator+(const bitwise<U>& x, const bitwise<U>& y) noexcept(noexcept(
      x.value() + y.value())) -> decltype(bitwise<U>{x.value() + y.value()})
  {
    return bitwise<U>{x.value() + y.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend consteval auto
  operator-(const bitwise<U>& x, const bitwise<U>& y) noexcept(noexcept(
      x.value() - y.value())) -> decltype(bitwise<U>{x.value() - y.value()})
  {
    return bitwise<U>{x.value() - y.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend consteval auto
  operator*(const bitwise<U>& x, const bitwise<U>& y) noexcept(noexcept(
      x.value() * y.value())) -> decltype(bitwise<U>{x.value() * y.value()})
  {
    return bitwise<U>{x.value() * y.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend consteval auto
  operator/(const bitwise<U>& x, const bitwise<U>& y) noexcept(noexcept(
      x.value() / y.value())) -> decltype(bitwise<U>{x.value() / y.value()})
  {
    return bitwise<U>{x.value() / y.value()};
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend constexpr auto
  operator<=>(const bitwise<U>& x, const bitwise<U>& y) noexcept(
      noexcept(x.value() <=> y.value())) -> decltype(x.value() <=> y.value())
  {
    return x.value() <=> y.value();
  }

  template <std::same_as<T> U>
  [[nodiscard]] friend constexpr auto
  operator==(const bitwise<U>& x, const bitwise<U>& y) noexcept(
      noexcept(x.value() == y.value())) -> decltype(x.value() == y.value())
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
