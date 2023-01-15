#include "constrained_value/constrained_value.hpp"
#include "utility.hpp"

#include <boost/ut.hpp>
#include <cmath>
#include <complex>
#include <numbers>
#include <ranges>

auto main() -> int
{
  namespace cnv = ::constrained_value;
  using namespace cnv::test;
  using namespace ::boost::ut;

  test("unit constrained_value") = [] {
    constexpr_valid<cnv::unit>(-1);
    constexpr_valid<cnv::unit>(+1);
    constexpr_valid<cnv::unit>(-1.0);
    constexpr_valid<cnv::unit>(+1.0);

    valid<cnv::unit>(std::complex{-1.0, 0.0});
    valid<cnv::unit>(std::complex{+1.0, 0.0});
    valid<cnv::unit>(std::complex{0.0, -1.0});
    valid<cnv::unit>(std::complex{0.0, +1.0});

    invalid<cnv::unit>(0);
    invalid<cnv::unit>(0.0);
    invalid<cnv::unit>(std::complex<double>{});
  };

#ifndef __clang__
  test("values on the unit circle") = [] {
    namespace rv = std::views;

    for (auto angle :
         rv::iota(0) | rv::transform([](auto i) {
           return static_cast<double>(i) / 1000.0;
         }) | rv::take_while([](double d) { return d < 2.0 * std::numbers::pi; })) {

      auto z = std::polar(1.0, angle);
      valid<cnv::unit>(z)
        << "ulp dist from 1: " << ulp_distance(1.0, std::abs(z))
        << "| angle: " << angle
        ;
    }
  };
#endif
}
