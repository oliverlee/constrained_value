#include "constrained_value/constrained_value.hpp"
#include "utility.hpp"

#include <boost/ut.hpp>

#include <cmath>
#include <numeric>

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
  namespace cnv = ::constrained_value;
  using namespace cnv::test;
  using namespace ::boost::ut;

  test("near constrained_value and int nttp") = [] {
    constexpr_valid<cnv::near, 0, 1>(+0.0);
    constexpr_valid<cnv::near, 0, 1>(-0.0);
    constexpr_valid<cnv::near, 0, 1>(+1.0);
    constexpr_valid<cnv::near, 0, 1>(-1.0);
    constexpr_valid<cnv::near, 0, 1>(+0.1);
    constexpr_valid<cnv::near, 0, 1>(-0.1);

    invalid<cnv::near, 0, 1>(std::nextafter(+1.0, +2.0));
    invalid<cnv::near, 0, 1>(std::nextafter(-1.0, -2.0));
    invalid<cnv::near, 0, 1>(+2.0);
    invalid<cnv::near, 0, 1>(-2.0);
  };

#if __cpp_nontype_template_args >= 201911
  test("near constrained_value with double nttp") = [] {
    constexpr_valid<cnv::near, 1.0, 0.1>(0.9);
    constexpr_valid<cnv::near, 1.0, 0.1>(1.0);
    constexpr_valid<cnv::near, 1.0, 0.1>(1.1);

    using cnv::constant::ulp;

    constexpr_valid<cnv::near, 1.0, ulp<1>>(1.0);
    constexpr_valid<cnv::near, 1.0, ulp<1>>(
        1.0 + std::numeric_limits<double>::epsilon());

    // smaller than 1.0 decreases the exponent resulting in higher resolution
    constexpr_valid<cnv::near, 1.0, ulp<2>>(
        1.0 - std::numeric_limits<double>::epsilon());

    invalid<cnv::near, 1.0, 0.1>(std::nextafter(1.0 + 0.1, +2.0));
    invalid<cnv::near, 1.0, 0.1>(std::nextafter(1.0 - 0.1, -2.0));
  };
#endif

  test("near constrained_value with bitwise constant nttp") = [] {
    using cnv::constant::_;

    constexpr_valid<cnv::near, _(1.0), _(0.1)>(0.9);
    constexpr_valid<cnv::near, _(1.0), _(0.1)>(1.0);
    constexpr_valid<cnv::near, _(1.0), _(0.1)>(1.1);

    invalid<cnv::near, _(1.0), _(0.1)>(std::nextafter(1.0 + 0.1, +2.0));
    invalid<cnv::near, _(1.0), _(0.1)>(std::nextafter(1.0 - 0.1, -2.0));
  };

#if __cpp_nontype_template_args >= 201911
  test("near constrained_value with double + ulp nttp") = [] {
    using cnv::constant::ulp;

    constexpr_valid<cnv::near, 1.0, ulp<1>>(1.0);
    constexpr_valid<cnv::near, 1.0, ulp<1>>(
        1.0 + std::numeric_limits<double>::epsilon());

    // smaller than 1.0 decreases the exponent resulting in higher resolution
    constexpr_valid<cnv::near, 1.0, ulp<2>>(
        1.0 - std::numeric_limits<double>::epsilon());
  };
#endif

  test("near constrained_value with bitwise + ulp nttp") = [] {
    using cnv::constant::ulp;
    using cnv::constant::_;

    constexpr_valid<cnv::near, _(1.0), ulp<1>>(1.0);
    constexpr_valid<cnv::near, _(1.0), ulp<1>>(
        1.0 + std::numeric_limits<double>::epsilon());
    constexpr_valid<cnv::near, _(1.0), ulp<2>>(
        1.0 - std::numeric_limits<double>::epsilon());
  };
}

// NOLINTEND(readability-magic-numbers)
