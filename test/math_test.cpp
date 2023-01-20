#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>

#include <bit>
#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>

auto main() -> int
{
  namespace cnv = ::constrained_value;
  using namespace ::boost::ut;

  test("math::isnan returns true if value is NaN") = [] {
    constexpr auto nan = std::numeric_limits<float>::quiet_NaN();

    expect(constant<cnv::math::isnan(nan)>);
    expect(not constant<cnv::math::isnan(std::bit_cast<std::int32_t>(nan))>);
  };

  test("math::signbit returns true if negative sign bit in IEC 559 bit "
       "representation") = [] {
    expect(constant<cnv::math::signbit(-0.0)>);
    expect(constant<cnv::math::signbit(-0.F)>);
    expect(constant<cnv::math::signbit(-1)>);

    // NOLINTBEGIN(readability-magic-numbers): highest bit set
    expect(constant<cnv::math::signbit(std::uint8_t{0xFFU})>);
    expect(constant<cnv::math::signbit(std::uint8_t{0x80U})>);
    // NOLINTEND(readability-magic-numbers): highest bit set

    expect(not constant<cnv::math::signbit(+0.0)>);
    expect(not constant<cnv::math::signbit(+0.F)>);
    expect(not constant<cnv::math::signbit(0)>);
    expect(not constant<cnv::math::signbit(1)>);

    // NOLINTBEGIN(readability-magic-numbers): highest bit *not* set
    expect(not constant<cnv::math::signbit(std::uint8_t{0x7FU})>);
    expect(not constant<cnv::math::signbit(std::uint8_t{0x00U})>);
    // NOLINTEND(readability-magic-numbers): highest bit *not* set
  };

  test("math::signum returns ordering as int") = [] {
    expect(constant<1_i == cnv::math::signum<int>(+1.0, 0.0)>);
    expect(constant<0_i == cnv::math::signum<int>(0.0, 0.0)>);
    expect(constant<0_i == cnv::math::signum<int>(-0.0, +0.0)>);
    expect(constant<-1_i == cnv::math::signum<int>(-1.0, 0.0)>);

    static_assert(
        std::same_as<int, decltype(cnv::math::signum<int>(0.0, 0.0))>);
    static_assert(
        std::same_as<std::int8_t,
                     decltype(cnv::math::signum<std::int8_t>(0.0, 0.0))>);
    static_assert(
        std::same_as<std::int64_t,
                     decltype(cnv::math::signum<std::int64_t>(0.0, 0.0))>);
  };

  test("math::signum aborts if values are not comparable") = [] {
    static const auto nan = 0.0 / 0.0;

    expect(aborts([] { return cnv::math::signum<int>(0.0, nan); }));
    expect(aborts([] { return cnv::math::signum<int>(nan, 0.0); }));
    expect(aborts([] { return cnv::math::signum<int>(nan, nan); }));
  };

  test("math::signum not invocable for non-three way comparable types") = [] {
    struct S
    {};

    static_assert(
        not std::is_invocable_v<decltype(cnv::math::signum<int>), S, S>);
  };
}
