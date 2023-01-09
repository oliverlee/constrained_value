#pragma once

#include "src/assert_predicate.hpp"
#include "src/predicate.hpp"
#include "src/violation_policy.hpp"

#include <concepts>
#include <functional>
#include <type_traits>

/// Define types with constraints
namespace constrained_value {

namespace detail {

/// A value that always satisfies an invariant
/// @tparam T underlying type
/// @tparam V invariant violation policy
/// @tparam P predicate describing a type invariant
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
// TODO allow multiple predicates
// TODO handle non-copyable types
template <std::copyable T, typename V, std::predicate<T> P>
  requires std::default_initializable<P> and
           violation_policy<V, T, P, source_location>
class constrained_value
{
  T value_;

public:
  /// Underlying type
  ///
  using underlying_type = T;

  /// Determine if a value satisfies the invariants of `constrained_value`
  ///
  static constexpr auto valid = P{};

  /// Default construct a constrained_value
  /// @requires `std::default_initializable<T>`
  /// @pre `T{}` satisfies `P`
  ///
  constexpr constrained_value(source_location sl = source_location::current())
    requires std::default_initializable<T>
      : value_{(assert_predicate<V>(T{}, valid, __PRETTY_FUNCTION__, sl), T{})}
  {}

  /// Construct a constrained_value
  /// @tparam U underlying type `T`
  /// @param value `value` of underlying type
  /// @pre value satisfies `P`
  /// @note requires `std::same_as<T, U>` to prevent implicit conversion of
  ///     `value` to `T`
  ///
  template <std::same_as<T> U>
  constexpr constrained_value(U value,
                              source_location sl = source_location::current())
      : value_{
            (assert_predicate<V>(value, valid, __PRETTY_FUNCTION__, sl), value)}
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
  [[nodiscard]] constexpr operator T() const
      noexcept(std::is_nothrow_copy_constructible_v<T>)
  {
    return value_;
  }
  /// @}
};

/// Obtain the constrained_value type
/// @tparam T underlying type
/// @tparam V invariant violation policy
/// @tparam P predicate
///
/// Allow a user to omit the invariant violation policy when specifying a
/// `constrained_value` type.
///
/// @{
template <typename T, typename V, typename P = void>
struct constrained_value_ : std::type_identity<constrained_value<T, V, P>>
{};

template <typename T, typename P>
struct constrained_value_<T, P, void>
    : std::type_identity<constrained_value<T, on_violation::print_and_abort, P>>
{};
/// @}

}  // namespace detail

/// @copydoc detail::constrained_value
template <typename... Xs>
using constrained_value = typename detail::constrained_value_<Xs...>::type;

/// A value always greater than zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T>
using positive = constrained_value<T, predicate::positive>;

}  // namespace constrained_value
