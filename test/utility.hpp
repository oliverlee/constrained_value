#pragma once

#include <boost/ut.hpp>

namespace constrained_value::test {

template <template <typename, auto...> typename constraint,
          auto... args,
          typename T>
consteval auto constexpr_valid(T value)
{
  (void)constraint<T, args...>{value};
}

template <template <typename, auto...> typename constraint,
          auto... args,
          typename T>
auto valid(T value)
{
  (void)constraint<T, args...>{value};
}

template <template <typename, auto...> typename constraint,
          auto... args,
          typename T>
auto invalid(T value)
{
  using ::boost::ut::aborts;
  using ::boost::ut::expect;

  expect(aborts([&] { (void)constraint<T, args...>{value}; }));
}

}  // namespace constrained_value::test
