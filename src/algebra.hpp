#pragma once

#include <concepts>
#include <functional>

// clang-format off

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace constrained_value {

/// Algebraic concepts used in predicate requirements
///
namespace algebra {

/// Disable associativity for a set and operation
///
/// A binary operation of elements in a set is assumed to be associative unless
/// this variable template is specialized to equal `true`.
///
template <typename S, typename Op>
constexpr auto disable_associativity = false;

/// Specifies that an operation over a set is associative
template <typename S, typename Op>
concept associative =
  std::regular_invocable<Op, S, S> and
  not disable_associativity<S, Op>;

/// Specifies that a set and a binary operation are closed
/// @see https://en.wikipedia.org/wiki/Magma_(algebra)
///
template <typename S, typename Op>
concept magma =
  std::regular_invocable<Op, S, S> and
  requires(S a, S b, Op op)
  {
    { std::invoke(op, a, b) } -> std::convertible_to<S>;
  };

/// Specifies that a set and a binary operation are closed and the operation is
/// associative
/// @see https://en.wikipedia.org/wiki/Semigroup
///
template <typename S, typename Op>
concept semigroup =
  magma<S, Op> and
  associative<S, Op>;

/// Specifies that a set and the addition operation forms a semigroup and
///     the set contains an additive identity element
/// @note Requires `S{}` to be the additive identity element.
/// @see https://en.wikipedia.org/wiki/Monoid
///
template <typename S>
concept additive_monoid =
  semigroup<S, std::plus<>> and
  std::default_initializable<S>;

/// Specifies that a set is an additive monoid and `-s` returns the additive
///     inverse for an element `s`
/// @see https://en.wikipedia.org/wiki/Group_(mathematics)
///
template <typename S>
concept additive_group =
  additive_monoid<S> and
  requires(S a)
  {
    { -a } -> std::convertible_to<S>;
  };

}  // namespace algebra
}  // namespace constrained_value

// clang-format on
