#pragma once

#include "src/adaptor.hpp"
#include "src/constant.hpp"

#include <functional>

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace constrained_value {

/// Common predicates used in defining type invariants
namespace predicate {

/// Checks if one value is less than another value
///
/// Binary predicate function object that applies `std::ranges::less` to two
/// values. This function object allows a non-type template parameter to be
/// bound to the invocation of `std::ranges::less`.
///
/// ~~~{.cpp}
/// less{}(+3.0, 0.0); // false
/// less{}(-3.0, 0.0); // true
/// ~~~
///
struct less : adaptor::nttp_bindable<std::ranges::less>
{};

/// Checks if one value is greater than another value
///
/// Binary predicate function object that applies `std::ranges::greater` to two
/// values. This function object allows a non-type template parameter to be
/// bound to the invocation of `std::ranges::greater`.
///
/// ~~~{.cpp}
/// greater{}(+3.0, 0.0); // true
/// greater{}(-3.0, 0.0); // false
/// ~~~
///
struct greater : adaptor::nttp_bindable<std::ranges::greater>
{};

/// Checks if a value is less than zero
///
/// Unary predicate function object used to determine if a value is negative.
/// For a value of type `T`, `T{}` is treated as zero.
///
/// ~~~{.cpp}
/// negative{}(+3.0); // false
/// negative{}(-3.0); // true
/// ~~~
///
struct negative : greater::bind_back<constant::Zero{}>
{};

/// Checks if a value is greater than zero
///
/// Unary predicate function object used to determine if a value is positive.
/// For a value of type `T`, `T{}` is treated as zero.
///
/// ~~~{.cpp}
/// positive{}(+3.0); // true
/// positive{}(-3.0); // false
/// ~~~
///
struct positive : greater::bind_back<constant::Zero{}>
{};

}  // namespace predicate

}  // namespace constrained_value
