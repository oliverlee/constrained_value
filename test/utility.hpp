#pragma once

#include <bit>
#include <boost/ut.hpp>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

namespace constrained_value::test {

/// Join tuples
///
template <typename... Tuples>
using join_t = decltype(std::tuple_cat(std::declval<Tuples>()...));

/// Rebind template arguments into another template
///
/// @{
template <template <typename...> typename dest,
          template <typename...>
          typename source,
          typename... Args>
auto rebind_args_into_impl(source<Args...>) -> dest<Args...>;

template <typename T, template <typename...> typename dest>
using rebind_args_into_t =
    decltype(rebind_args_into_impl<dest>(std::declval<T>()));
/// @}

/// Replacement policy that throws
///
/// @{
struct invariant_error
{};
using throw_policy = decltype([](auto&&...) { throw invariant_error{}; });
/// @}

/// ULP/bit distance between two floating point values
///
template <std::floating_point T, std::same_as<T> U>
  requires(std::numeric_limits<T>::is_iec559)
auto ulp_distance(T a, U b)
{
  static constexpr auto isnan = [](auto x) { return x != x; };

  assert(not isnan(a));
  assert(not isnan(b));

  // assume T is not long double
  using I =
      std::conditional_t<std::same_as<float, T>, std::uint32_t, std::uint64_t>;

  const auto x = std::bit_cast<I>(a);
  const auto y = std::bit_cast<I>(b);

  return std::min(x - y, y - x);
}

/// Test that a constrained_value is constructible at compile time without an
/// invariant violation
///
template <template <typename, auto...> typename constraint,
          auto... args,
          typename T>
consteval auto constexpr_valid(T value)
{
  (void)constraint<T, args...>{value};
}

/// Test that a constrained_value is constructible without an invariant
/// violation
///
template <template <typename, auto...> typename constraint,
          auto... args,
          typename T>
auto valid(T value)
{
  using ::boost::ut::expect;
  using ::boost::ut::nothrow;

  using C1 = constraint<T, args...>;
  using C2 = rebind_args_into_t<
      join_t<std::tuple<T, throw_policy>, typename C1::predicate_types>,
      detail::constrained_value>;

  return expect(nothrow([&] { C2{value}; }));
}

/// Test that the construction of a constrained_value fail due to an invariant
/// violation
///
template <template <typename, auto...> typename constraint,
          auto... args,
          typename T>
auto invalid(T value)
{
  using ::boost::ut::aborts;
  using ::boost::ut::expect;

  return expect(aborts([&] { (void)constraint<T, args...>{value}; }));
}

}  // namespace constrained_value::test
