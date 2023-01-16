#pragma once

#include "src/assert_predicate.hpp"
#include "src/ulp_distance.hpp"
#include "src/violation_policy.hpp"

#include <concepts>
#include <type_traits>
#include <utility>

namespace constrained_value {

/// A value that always satisfies an invariant
/// @tparam T underlying type
/// @tparam P predicates describing a type invariant
/// @tparam V invariant violation policy
///
/// `constrained_value` wraps an underlying type `T` while validating that the
/// contained value satisfies an invariant specified by `P`. On construction or
/// assignment of an invalid value (i.e. a value that does not satisfy `P`), a
/// violation handler is invoked. The default behavior invokes `std::abort`.
///
/// This type provides a converting constructor from the underlying type,
/// allowing ergonomic use as arguments to functions or data members of an
/// aggregate.
///
/// ~~~{.cpp}
/// auto scale(positive<double> arg);
/// ...
/// scale(0.1);
/// ~~~
///
/// ~~~{.cpp}
/// stuct options
/// {
///   positive<double> alpha;
///   ...
/// };
/// ...
/// options{.alpha = 0.1, ...};
/// ~~~
///
// TODO handle non-copyable types
template <
    std::copyable T,
    std::predicate<T> P,
    violation_policy<T, P, source_location> V = on_violation::print_and_abort>
  requires (
      std::same_as<T, std::remove_cvref_t<T>> and std::default_initializable<P>)
class constrained_value
{
  T value_;

public:
  /// Underlying type
  ///
  using underlying_type = T;

  /// Predicate type
  ///
  using predicate_type = P;

  /// Violation policy type
  ///
  using violation_policy_type = V;

  /// Default construct a constrained_value
  /// @requires `std::default_initializable<T>`
  /// @pre `T{}` satisfies `P`
  ///
  constexpr constrained_value(source_location sl = source_location::current())
    requires std::default_initializable<T>
      : value_{(assert_predicate<P, V>(T{}, __PRETTY_FUNCTION__, sl), T{})}
  {}

  /// Construct a constrained_value
  /// @tparam U underlying type `T`
  /// @param value `value` of underlying type
  /// @pre value satisfies `P`
  /// @note requires `std::same_as<T, U>` to prevent implicit conversion of
  ///     `value` to `T`
  ///
  template <std::same_as<T> U>
  constexpr constrained_value(
      U value, source_location sl = source_location::current())
      : value_{(assert_predicate<P, V>(value, __PRETTY_FUNCTION__, sl), value)}
  {}

  /// Return a reference to the underlying value
  /// @{
  [[nodiscard]] constexpr auto value() & noexcept -> const T& { return value_; }
  [[nodiscard]] constexpr auto value() const& noexcept -> const T&
  {
    return value_;
  }
  /// @}

  /// Return the underlying value
  /// @{
  [[nodiscard]] constexpr auto
  value() && noexcept(std::is_nothrow_move_constructible_v<T>) -> T
  {
    return std::move(value_);
  }
  [[nodiscard]] constexpr auto
  value() const&& noexcept(std::is_nothrow_copy_constructible_v<T>) -> T
  {
    return value_;
  }
  /// @}

  /// Implicit conversion to the underlying type
  /// @{
  [[nodiscard]] constexpr
  operator T() const noexcept(std::is_nothrow_copy_constructible_v<T>)
  {
    return value_;
  }
  /// @}
};

/// Checks if a type is a specialization of `constrained_value`
/// @{
template <typename...>
inline constexpr auto is_constrained_value_v = false;
template <typename... Ts>
inline constexpr auto is_constrained_value_v<constrained_value<Ts...>> = true;
/// @}

}  // namespace constrained_value
