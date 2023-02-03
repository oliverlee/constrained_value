#pragma once

#include <boost/ut.hpp>
#include <type_traits>

namespace constrained_value::test {

/// Replacement policy that throws
///
/// @{
struct invariant_error
{};
inline constexpr auto throw_policy = [](auto&&...) { throw invariant_error{}; };
/// @}

/// Test that a constrained_value is constructible at compile time without an
/// invariant violation
///
template <
    template <typename, auto...>
    typename constraint,
    auto... args,
    typename T>
consteval auto constexpr_valid(T value)
{
  (void)constraint<T, args...>{value};
}

/// Test that a constrained_value is constructible without an invariant
/// violation
///
template <
    template <typename, auto...>
    typename constraint,
    auto... args,
    typename T>
auto valid(T value)
{
  using ::boost::ut::expect;
  using ::boost::ut::nothrow;

  using C = constrained_value<
    T,
    typename constraint<T, args...>::predicate_type,
    std::remove_cvref_t<decltype(throw_policy)>
    >;

  return expect(nothrow([&] { C{value}; }));
}

/// Test that the construction of a constrained_value fail due to an invariant
/// violation
///
template <
    template <typename, auto...>
    typename constraint,
    auto... args,
    typename T>
auto invalid(T value)
{
  using ::boost::ut::aborts;
  using ::boost::ut::expect;

  return expect(aborts([&] { (void)constraint<T, args...>{value}; }));
}

}  // namespace constrained_value::test
