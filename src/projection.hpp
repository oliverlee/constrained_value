#pragma once

#include "src/detail/priority.hpp"

#include <concepts>

namespace constrained_value::projection {
namespace detail {

class abs_fn
{
  template <int N>
  using priority = ::constrained_value::detail::priority<N>;

  template <std::integral T>
  static constexpr auto impl(priority<1>, T value) noexcept -> T
  {
    // abs(INT_MIN) is UB if T is two's complement
    return value < T{} ? -value : value;
  }

  template <std::floating_point T>
  static constexpr auto impl(priority<1>, T value) noexcept -> T
  {
    // If the argument is ±0, +0 is returned
    if (value == T{}) {
      return T{};
    }

    // If the argument is NaN, NaN is returned
    // NOLINTNEXTLINE(misc-redundant-expression)
    if (value != value) {
      return value;
    }

    return value < T{} ? -value : value;
  }

  template <typename T>
  static constexpr auto
  impl(priority<0>, T&& value) noexcept(noexcept(abs(std::forward<T>(value))))
      -> decltype(abs(std::forward<T>(value)))
  {
    return abs(std::forward<T>(value));
  }

public:
  template <typename T>
  constexpr auto operator()(T&& value) const
      -> decltype(impl(priority<1>{}, std::forward<T>(value)))
  {
    return impl(priority<1>{}, std::forward<T>(value));
  }
};

}  // namespace detail

using abs = detail::abs_fn;

}  // namespace constrained_value::projection
