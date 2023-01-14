#pragma once

#include <compare>
#include <concepts>
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
  [[nodiscard]] constexpr operator T() const
      noexcept(std::is_nothrow_default_constructible_v<T>)
  {
    return T{};
  }

  /// Defaulted comparisons
  /// @{
  [[nodiscard]] friend auto operator<=>(const Zero&, const Zero&) = default;
  [[nodiscard]] friend auto operator==(const Zero&, const Zero&)
      -> bool = default;
  /// @}

  // NOTE: explicit return types result in recursive template instantiation and
  // stack exhaustion with clang.
  // Comparability must be checked after determining T and Zero are different
  // types.

  /// Comparisons against arbitrary types
  /// @{
  template <std::default_initializable T>
    requires(not std::same_as<T, Zero> and std::equality_comparable<T>)
  [[nodiscard]] friend constexpr auto
  operator<=>(const T& x, const Zero&) noexcept(noexcept(x <=> T{}))
  {
    return x <=> T{};
  }

  template <std::default_initializable T>
    requires(not std::same_as<T, Zero> and std::three_way_comparable<T>)
  [[nodiscard]] friend constexpr auto
  operator==(const T& x, const Zero&) noexcept(noexcept(x == T{}))
  {
    return x == T{};
  }
  /// @}
};

}  // namespace constant

}  // namespace constrained_value
