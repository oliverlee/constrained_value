#pragma once

#include "src/assert_predicate.hpp"
#include "src/predicate.hpp"

#include <concepts>
#include <functional>
#include <type_traits>

/// Define types with constraints
namespace constrained_value {

namespace detail {

/// A value that always satisfies an invariant
/// @tparam T underlying type
/// @tparam V invariant violation policy
/// @tparam Ps predicates describing a type invariant
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
template <std::copyable T, typename V, std::predicate<T>... Ps>
  requires(sizeof...(Ps) != 0 and (std::default_initializable<Ps> and ...))
class constrained_value
{
  T value_;

public:
  /// Underlying type
  ///
  using underlying_type = T;

  /// Determine if a value satisfies the invariants of `constrained_value`
  ///
  static constexpr auto valid(const T& value)  //
      noexcept(noexcept(assert_predicates<on_violation::ignore, Ps...>(value)))
          -> bool
  {
    return assert_predicates<on_violation::ignore, Ps...>(value);
  }

  /// Default construct a constrained_value
  /// @requires `std::default_initializable<T>`
  /// @pre `T{}` satisfies `P`
  ///
  constexpr constrained_value(source_location sl = source_location::current())
    requires std::default_initializable<T>
      : value_{(assert_predicates<V, Ps...>(T{}, __PRETTY_FUNCTION__, sl), T{})}
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
      : value_{(assert_predicates<V, Ps...>(value, __PRETTY_FUNCTION__, sl),
                value)}
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
template <bool, typename T, typename V, typename... Ps>
struct constrained_value_ : std::type_identity<constrained_value<T, V, Ps...>>
{};

template <typename T, typename P0, typename... Ps>
struct constrained_value_<true, T, P0, Ps...>
    : std::type_identity<
          constrained_value<T, on_violation::print_and_abort, P0, Ps...>>
{};
/// @}

}  // namespace detail

/// @copydoc detail::constrained_value
template <typename T, typename V, typename... Ps>
using constrained_value = typename detail::
    constrained_value_<std::predicate<V, T>, T, V, Ps...>::type;

/// A value always greater than zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T>
using positive = constrained_value<T, predicate::positive>;

/// A value strictly contained by lower and upper bounds
/// @tparam T underlying type
/// @tparam lo strict lower bound
/// @tparam hi strict upper bound
///
template <std::totally_ordered T, auto lo, auto hi>
using strictly_bounded =
    constrained_value<T,
                      predicate::greater::bind_back<lo>,
                      predicate::less::bind_back<hi>>;

}  // namespace constrained_value
