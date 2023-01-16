#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>

#include <cmath>
#include <concepts>
#include <limits>

namespace cnv = ::constrained_value;

template <std::floating_point T, std::signed_integral I>
constexpr auto ulp_offset(T value, I offset)
{
  const auto inf = std::numeric_limits<T>::infinity();

  for (; offset > I{}; --offset) {
    value = std::nextafter(value, +inf);
  }

  while (offset++ < I{}) {
    value = std::nextafter(value, -inf);
  }

  return value;
}

auto main() -> int
{
  using namespace ::boost::ut;

  test("ulp_distance around 0") = [] {
    expect(constant<0_i == cnv::ulp_distance(+0.0, +0.0)>);
    expect(constant<0_i == cnv::ulp_distance(-0.0, -0.0)>);
    expect(constant<0_i == cnv::ulp_distance(-0.0, +0.0)>);
    expect(constant<0_i == cnv::ulp_distance(-0.0, -0.0)>);

    const auto a = ulp_offset(+0.0, 1);

    expect(1_i == cnv::ulp_distance(+0.0, a));
    expect(1_i == cnv::ulp_distance(-0.0, a));
    expect(-1_i == cnv::ulp_distance(+0.0, -a));
    expect(-1_i == cnv::ulp_distance(-0.0, -a));

    expect(2_i == cnv::ulp_distance(-a, +a));
    expect(2_i == cnv::ulp_distance(+0.0, a + a));
    expect(2_i == cnv::ulp_distance(-0.0, a + a));

    expect(-2_i == cnv::ulp_distance(+a, -a));
    expect(-2_i == cnv::ulp_distance(a + a, +0.0));
    expect(-2_i == cnv::ulp_distance(a + a, -0.0));

    const auto b = ulp_offset(+0.0, 2);

    expect(2_i == cnv::ulp_distance(+0.0, b));
    expect(2_i == cnv::ulp_distance(-0.0, b));
    expect(-2_i == cnv::ulp_distance(+0.0, -b));
    expect(-2_i == cnv::ulp_distance(-0.0, -b));

    expect(4_i == cnv::ulp_distance(-b, +b));
    expect(4_i == cnv::ulp_distance(+0.0, b + b));
    expect(4_i == cnv::ulp_distance(-0.0, b + b));
  };

  test("ulp_distance around 1") = [] {
    const auto hi = std::nextafter(+1.0, +2.0);

    expect(1_i == cnv::ulp_distance(+1.0, hi));
    expect(-1_i == cnv::ulp_distance(hi, +1.0));

    const auto lo = std::nextafter(+1.0, -2.0);

    expect(-1_i == cnv::ulp_distance(+1.0, lo));
    expect(1_i == cnv::ulp_distance(lo, +1.0));

    expect(2_i == cnv::ulp_distance(lo, hi));
    expect(-2_i == cnv::ulp_distance(hi, lo));
    expect(-2_i == cnv::ulp_distance(-lo, -hi));
    expect(2_i == cnv::ulp_distance(-hi, -lo));
  };

  test("ulp_distance between zero and min") = [] {
    constexpr auto num_mantissa_values = detail::integral_constant<
        std::int64_t{1} << (std::numeric_limits<double>::digits - 1)>{};

    constexpr auto min = std::numeric_limits<double>::min();

    expect(constant<num_mantissa_values == cnv::ulp_distance(+0.0, min)>);
    expect(constant<num_mantissa_values == cnv::ulp_distance(-0.0, min)>);
    expect(constant<-num_mantissa_values == cnv::ulp_distance(min, +0.0)>);
    expect(constant<-num_mantissa_values == cnv::ulp_distance(min, -0.0)>);
    expect(constant<-num_mantissa_values == cnv::ulp_distance(+0.0, -min)>);
    expect(constant<-num_mantissa_values == cnv::ulp_distance(-0.0, -min)>);

    constexpr auto twice_num_mantissa_values = detail::integral_constant<
        std::int64_t{2} * num_mantissa_values.get()>{};

    expect(
        constant<twice_num_mantissa_values == cnv::ulp_distance(-min, +min)>);
    expect(
        constant<-twice_num_mantissa_values == cnv::ulp_distance(+min, -min)>);
  };
}
