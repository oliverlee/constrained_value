#pragma once

#include "src/adaptor.hpp"
#include "src/constant.hpp"
#include "src/detail/priority.hpp"

#include <complex>
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
/// equal_to{}(3.0, 0.0); // false
/// ~~~
///
struct equal_to : adaptor::nttp_bindable<std::ranges::equal_to>
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

namespace detail {

class constexpr_abs_fn
{
  template <int N>
  using priority = ::constrained_value::detail::priority<N>;

  // `abs` is not yet constexpr with Clang
  template <typename T>
  static constexpr auto impl(priority<3>, const std::complex<T>& z)  //
      noexcept                                                       //
      -> T
  {
    return std::real(z) * std::real(z) + std::imag(z) * std::imag(z);
  }

  // `abs` is not constexpr until C++23
  template <std::floating_point T>
  static constexpr auto impl(priority<2>, const T& z)  //
      noexcept                                         //
      -> T
  {
    // If the argument is ±0, +0 is returned
    if (z == T{}) {
      return T{};
    }

    // If the argument is NaN, NaN is returned
    if (z != z) {
      return z;
    }

    return z < T{} ? -z : z;
  }

  // `abs` is not constexpr until C++23
  template <std::integral T>
  static constexpr auto impl(priority<1>, const T& z)  //
      noexcept                                         //
      -> T
  {
    // abs(INT_MIN) is already UB
    return z < T{} ? -z : z;
  }

  static constexpr auto impl(priority<0>, const auto& z)  //
      noexcept(noexcept(abs(z)))                          //
      -> decltype(abs(z))
  {
    return abs(z);
  }

public:
  constexpr auto operator()(const auto& z) const
      noexcept(noexcept(impl(priority<3>{}, z)))
          -> decltype(impl(priority<3>{}, z))
  {
    return impl(priority<3>{}, z);
  }
};

}  // namespace detail

/// Checks if a value has an absolute value of exactly one
///
/// Unary predicate function object used to determine if the absolute value of a
/// value is equal to one. For a value of type `T`, `T{1}` is treated as one.
///
/// For a value `t` or type `T`, `abs(t)` must be a valid expression.
///
/// ~~~{.cpp}
/// unity{}(+1); // true
/// unity{}(-1); // true
/// unity{}(0);  // false
///
/// unity{}(+1.0); // true
/// unity{}(-1.0); // true
/// unity{}(0.0);  // false
///
/// unity{}(std::complex{1.0, 0.0}); // true
/// unity{}(std::complex{0.0, 1.0}); // true
/// unity{}(std::complex{0.0, 0.0}); // false
/// ~~~
///
struct unity
    : adaptor::projection<equal_to::bind_back<1>, detail::constexpr_abs_fn>
{};

}  // namespace predicate
}  // namespace constrained_value
