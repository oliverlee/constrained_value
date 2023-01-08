#pragma once

#include "src/assert_predicate.hpp"
#include "src/predicate.hpp"

#include <concepts>
#include <functional>

/// Define types with constraints
namespace constrained_value {

/// A value that always satisfies an invariant
/// @tparam T underlying type
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
// TODO specify violation policy
// TODO allow multiple predicates
// TODO handle non-copyable types
template <std::copyable T, std::predicate<T> P>
  requires std::default_initializable<P>
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
      : value_{(assert_predicate(T{}, valid, __PRETTY_FUNCTION__, sl), T{})}
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
      : value_{(assert_predicate(value, valid, __PRETTY_FUNCTION__, sl), value)}
  {}
};

/// A value always greater than zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T>
using positive = constrained_value<T, predicate::positive>;

}  // namespace constrained_value
