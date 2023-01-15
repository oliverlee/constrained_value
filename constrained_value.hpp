#pragma once

#include "src/algebra.hpp"
#include "src/constrained_value.hpp"
#include "src/functional.hpp"
#include "src/predicate.hpp"
#include "src/projection.hpp"
#include "src/violation_policy.hpp"

#include <concepts>
#include <type_traits>
#include <utility>

/// Define types with constraints
namespace constrained_value {

/// Verify that a value satisfies a constraint
/// @tparam constraint specialization of `constrained_value`
/// @tparam S explicitly specified constrained_value underlying type
/// @tparam U value type
/// @tparam T constrained_value underlying type
///
/// Forwards a value if it satisfies the specified constraint, otherwise invokes
/// the associated violation policy.
///
/// In order to handle wrapper types (e.g. to allow floating point value as
/// non-type template parameters with Clang), the constrained_value underlying
/// type may be explicitly specified with `S`. If `S` is set to a non-`void`
/// type, the underlying type `T` is set to `S`. Otherwise, `T` is deduced from
/// `U`.
///
template <template <typename> typename constraint,
          typename S = void,
          typename U,
          typename T = std::conditional_t<std::is_void_v<S>, U, S>>
  requires(is_constrained_value_v<constraint<std::remove_cvref_t<T>>> and
           std::convertible_to<U, T>)
constexpr auto ensure(U&& value) -> U&&
{
  if (not constraint<T>::valid(static_cast<T>(value))) {
    (void)constraint<T>{static_cast<T>(value)};
  }
  return std::forward<U>(value);
}

/// A value always less than zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T,
          auto violation_policy = on_violation::print_and_abort{}>
using negative =
    constrained_value<T, predicate::negative, decltype(violation_policy)>;

/// A value always greater than or equal to zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T,
          auto violation_policy = on_violation::print_and_abort{}>
using nonnegative =
    constrained_value<T, predicate::nonnegative, decltype(violation_policy)>;

/// A value always greater than zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T,
          auto violation_policy = on_violation::print_and_abort{}>
using positive =
    constrained_value<T, predicate::positive, decltype(violation_policy)>;

/// A value always less than or equal to zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T,
          auto violation_policy = on_violation::print_and_abort{}>
using nonpositive =
    constrained_value<T, predicate::nonpositive, decltype(violation_policy)>;

/// A value equal to another value
/// @tparam T underlying type
/// @tparam arg
///
template <std::equality_comparable T,
          auto arg,
          auto violation_policy = on_violation::print_and_abort{}>
using equal_to =
    constrained_value<T,
                      predicate::equal_to::bind_back<arg>,
                      decltype(violation_policy)>;

/// A value not equal to anarg value
/// @tparam T underlying type
/// @tparam arg
///
template <std::equality_comparable T,
          auto arg,
          auto violation_policy = on_violation::print_and_abort{}>
using not_equal_to =
    constrained_value<T,
                      predicate::not_equal_to::bind_back<arg>,
                      decltype(violation_policy)>;

/// A value less than a supremum
/// @tparam T underlying type
/// @tparam sup
///
template <std::totally_ordered T,
          auto sup,
          auto violation_policy = on_violation::print_and_abort{}>
using less =
    constrained_value<T,
                      predicate::less::bind_back<sup>,
                      decltype(violation_policy)>;

/// A value less than or equal to a maximum
/// @tparam T underlying type
/// @tparam max
///
template <std::totally_ordered T,
          auto max,
          auto violation_policy = on_violation::print_and_abort{}>
using less_equal =
    constrained_value<T,
                      predicate::less_equal::bind_back<max>,
                      decltype(violation_policy)>;

/// A value greater than an infimum
/// @tparam T underlying type
/// @tparam inf
///
template <std::totally_ordered T,
          auto inf,
          auto violation_policy = on_violation::print_and_abort{}>
using greater =
    constrained_value<T,
                      predicate::greater::bind_back<inf>,
                      decltype(violation_policy)>;

/// A value greater than or equal to a minumum
/// @tparam T underlying type
/// @tparam min
///
template <std::totally_ordered T,
          auto min,
          auto violation_policy = on_violation::print_and_abort{}>
using greater_equal =
    constrained_value<T,
                      predicate::greater_equal::bind_back<min>,
                      decltype(violation_policy)>;

/// A value contained by lower and upper bounds
/// @tparam T underlying type
/// @tparam lo lower bound
/// @tparam hi upper bound
///
template <std::totally_ordered T,
          auto lo,
          auto hi,
          auto violation_policy = on_violation::print_and_abort{}>
using bounded = constrained_value<
    T,
    functional::all_of<predicate::greater_equal::bind_back<lo>,
                       predicate::less_equal::bind_back<hi>>,
    decltype(violation_policy)>;

/// A value strictly contained by lower and upper bounds
/// @tparam T underlying type
/// @tparam lo strict lower bound
/// @tparam hi strict upper bound
///
template <std::totally_ordered T,
          auto lo,
          auto hi,
          auto violation_policy = on_violation::print_and_abort{}>
using strictly_bounded =
    constrained_value<T,
                      functional::all_of<predicate::greater::bind_back<lo>,
                                         predicate::less::bind_back<hi>>,
                      decltype(violation_policy)>;

/// A value contained by lower and upper bounds calculated from another value
///     and an absolute tolerance
/// @tparam T underlying type
/// @tparam arg
/// @tparam tol nonnegative absolute tolerance
///

template <algebra::additive_group T,
          auto arg,
          auto tol,
          auto violation_policy = on_violation::print_and_abort{}>
using near = constrained_value<
    T,
    functional::all_of<predicate::greater_equal::bind_back<arg - tol>,
                       predicate::less_equal::bind_back<arg + tol>>,
    decltype(violation_policy)>;

/// A value with magnitude one
/// @tparam T underlying type
///
/// Adds an invariant to a type where the magnitude of the value must always
/// equal one. `T{1}` is treated as one.
///
/// `abs(t)` must be a valid expression and is used to determine the magnitude
/// value `t` of type `T`.
///
template <typename T, auto violation_policy = on_violation::print_and_abort{}>
using unit = constrained_value<
    T,
    functional::compose<projection::abs, predicate::equal_to::bind_back<1>>,
    decltype(violation_policy)>;

}  // namespace constrained_value
