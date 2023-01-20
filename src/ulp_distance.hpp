#pragma once

#include "src/bitwise_integer.hpp"
#include "src/compare.hpp"
#include "src/math.hpp"

#include <cassert>
#include <type_traits>

namespace constrained_value {

/// ULP/bit distance between two "floating point"-like values
///
template <math::iec559_floating_point T>
  requires bitwise_integer_reinterpretable<T>
[[nodiscard]] constexpr auto
ulp_distance(T a, T b) noexcept(is_nothrow_partial_order_comparable_v<T>)
{
  assert(math::isfinite(a));
  assert(math::isfinite(b));

  const auto x = bitwise_integer_reinterpretation(math::abs(a));
  const auto y = bitwise_integer_reinterpretation(math::abs(b));

  const auto dist =
      (math::signbit(a) == math::signbit(b)) ? math::abs(x - y) : (x + y);

  return math::signum<std::remove_const_t<decltype(x)>>(b, a) * dist;
}

}  // namespace constrained_value
