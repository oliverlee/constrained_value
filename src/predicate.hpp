#pragma once

#include "src/adaptor.hpp"
#include "src/constant.hpp"

#include <functional>

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace constrained_value {

/// Common predicates used in defining type invariants
namespace predicate {

/// Checks if a value is equal to another value
///
/// Binary predicate function object that applies `std::ranges::equal_to`
/// to two values. This function object allows a non-type template parameter to
/// be bound to the invocation of `std::ranges::equal_to`.
///
/// ~~~{.cpp}
/// equal_to{}(3.0, 3.0); // true
/// equal_to{}(3.0, 3);   // true
/// equal_to{}(3.0, 2.9); // false
/// equal_to{}(3.0, 3.1); // false
/// ~~~
///
struct equal_to : adaptor::nttp_bindable<std::ranges::equal_to>
{};

/// Checks if a value is not equal to another value
///
/// Binary predicate function object that applies `std::ranges::not_equal_to` to
/// two values. This function object allows a non-type template parameter to be
/// bound to the invocation of `std::ranges::not_equal_to`.
///
/// ~~~{.cpp}
/// not_equal_to{}(3.0, 3.0); // false
/// not_equal_to{}(3.0, 3);   // false
/// not_equal_to{}(3.0, 2.9); // true
/// not_equal_to{}(3.0, 3.1); // true
/// ~~~
///
struct not_equal_to : adaptor::nttp_bindable<std::ranges::not_equal_to>
{};

/// Checks if one value is less than another value
///
/// Binary predicate function object that applies `std::ranges::less` to two
/// values. This function object allows a non-type template parameter to be
/// bound to the invocation of `std::ranges::less`.
///
/// ~~~{.cpp}
/// less{}(+3.0, 0.0); // false
/// less{}(+0.0, 0.0); // false
/// less{}(-0.0, 0.0); // false
/// less{}(-3.0, 0.0); // true
/// ~~~
///
struct less : adaptor::nttp_bindable<std::ranges::less>
{};

/// Checks if one value is less than or equal to another value
///
/// Binary predicate function object that applies `std::ranges::less_equal` to
/// two values. This function object allows a non-type template parameter to be
/// bound to the invocation of `std::ranges::less_equal`.
///
/// ~~~{.cpp}
/// less_equal{}(+3.0, 0.0); // false
/// less_equal{}(+0.0, 0.0); // true
/// less_equal{}(-0.0, 0.0); // true
/// less_equal{}(-3.0, 0.0); // true
/// ~~~
///
struct less_equal : adaptor::nttp_bindable<std::ranges::less_equal>
{};

/// Checks if one value is greater than another value
///
/// Binary predicate function object that applies `std::ranges::greater` to two
/// values. This function object allows a non-type template parameter to be
/// bound to the invocation of `std::ranges::greater`.
///
/// ~~~{.cpp}
/// greater{}(+3.0, 0.0); // true
/// greater{}(+0.0, 0.0); // false
/// greater{}(-0.0, 0.0); // false
/// greater{}(-3.0, 0.0); // false
/// ~~~
///
struct greater : adaptor::nttp_bindable<std::ranges::greater>
{};

/// Checks if one value is greater than or equal to another value
///
/// Binary predicate function object that applies `std::ranges::greater_equal`
/// to two values. This function object allows a non-type template parameter to
/// be bound to the invocation of `std::ranges::greater_equal`.
///
/// ~~~{.cpp}
/// greater_equal{}(+3.0, 0.0); // true
/// greater_equal{}(+0.0, 0.0); // true
/// greater_equal{}(-0.0, 0.0); // true
/// greater_equal{}(-3.0, 0.0); // false
/// ~~~
///
struct greater_equal : adaptor::nttp_bindable<std::ranges::greater_equal>
{};

/// Checks if a value is less than zero
///
/// Unary predicate function object used to determine if a value is negative.
/// For a value of type `T`, `T{}` is treated as zero.
///
/// ~~~{.cpp}
/// negative{}(+3.0); // false
/// negative{}(+0.0); // false
/// negative{}(-0.0); // false
/// negative{}(-3.0); // true
/// ~~~
///
struct negative : less::bind_back<constant::Zero{}>
{};

/// Checks if a value is greater than or equal to zero
///
/// Unary predicate function object used to determine if a value is nonnegative.
/// For a value of type `T`, `T{}` is treated as zero.
///
/// ~~~{.cpp}
/// nonnegative{}(+3.0); // true
/// nonnegative{}(+0.0); // true
/// nonnegative{}(-0.0); // true
/// nonnegative{}(-3.0); // false
/// ~~~
///
struct nonnegative : greater_equal::bind_back<constant::Zero{}>
{};

/// Checks if a value is greater than zero
///
/// Unary predicate function object used to determine if a value is positive.
/// For a value of type `T`, `T{}` is treated as zero.
///
/// ~~~{.cpp}
/// positive{}(+3.0); // true
/// positive{}(+0.0); // false
/// positive{}(-0.0); // false
/// positive{}(-3.0); // false
/// ~~~
///
struct positive : greater::bind_back<constant::Zero{}>
{};

/// Checks if a value is less than or equal to zero
///
/// Unary predicate function object used to determine if a value is positive.
/// For a value of type `T`, `T{}` is treated as zero.
///
/// ~~~{.cpp}
/// nonpositive{}(+3.0); // false
/// nonpositive{}(+0.0); // true
/// nonpositive{}(-0.0); // true
/// nonpositive{}(-3.0); // true
/// ~~~
///
struct nonpositive : less_equal::bind_back<constant::Zero{}>
{};

}  // namespace predicate

}  // namespace constrained_value
