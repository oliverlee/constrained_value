#include "constrained_value/constrained_value.hpp"
#include "utility.hpp"

#include <boost/ut.hpp>
#include <cmath>

// positive test constant
static constexpr auto alpha = double{10};

auto main() -> int
{
  namespace cnv = ::constrained_value;
  using namespace cnv::test;
  using namespace ::boost::ut;

  test("negative constrained_value") = [] {
    constexpr_valid<cnv::negative>(-alpha);
    invalid<cnv::negative>(-0.0);
    invalid<cnv::negative>(+0.0);
    invalid<cnv::negative>(+alpha);
  };

  test("nonnegative constrained_value") = [] {
    invalid<cnv::nonnegative>(-alpha);
    constexpr_valid<cnv::nonnegative>(-0.0);
    constexpr_valid<cnv::nonnegative>(+0.0);
    constexpr_valid<cnv::nonnegative>(+alpha);
  };

  test("positive constrained_value") = [] {
    invalid<cnv::positive>(-alpha);
    invalid<cnv::positive>(-0.0);
    invalid<cnv::positive>(+0.0);
    constexpr_valid<cnv::positive>(+alpha);
  };

  test("nonpositive constrained_value") = [] {
    constexpr_valid<cnv::nonpositive>(-alpha);
    constexpr_valid<cnv::nonpositive>(-0.0);
    constexpr_valid<cnv::nonpositive>(+0.0);
    invalid<cnv::nonpositive>(+alpha);
  };

  test("equal to constrained_value") = [] {
    constexpr_valid<cnv::equal_to, 0>(-0.0);
    constexpr_valid<cnv::equal_to, 0>(+0.0);

    constexpr_valid<cnv::equal_to, cnv::constant::Zero{}>(-0.0);
    constexpr_valid<cnv::equal_to, cnv::constant::Zero{}>(+0.0);

    invalid<cnv::equal_to, 0>(-alpha);
    invalid<cnv::equal_to, 0>(+alpha);

    invalid<cnv::equal_to, 0>(std::nextafter(0.0, +1.0));
    invalid<cnv::equal_to, 0>(std::nextafter(0.0, -1.0));
  };

  test("not equal to constrained_value") = [] {
    constexpr_valid<cnv::not_equal_to, 0>(-1.0);
    constexpr_valid<cnv::not_equal_to, 0>(+1.0);

    invalid<cnv::not_equal_to, 0>(0.0);
    invalid<cnv::not_equal_to, cnv::constant::Zero{}>(0.0);

    valid<cnv::not_equal_to, 0>(std::nextafter(0.0, +1.0));
    valid<cnv::not_equal_to, 0>(std::nextafter(0.0, -1.0));
  };

  test("less constrained_value") = [] {
    constexpr_valid<cnv::less, 1>(-alpha);
    constexpr_valid<cnv::less, 1>(0.0);
    invalid<cnv::less, 1>(1.0);
    invalid<cnv::less, 1>(alpha);
  };

  test("less_equal constrained_value") = [] {
    constexpr_valid<cnv::less_equal, 1>(-alpha);
    constexpr_valid<cnv::less_equal, 1>(0.0);
    constexpr_valid<cnv::less_equal, 1>(1.0);
    invalid<cnv::less_equal, 1>(alpha);
  };

  test("greater constrained_value") = [] {
    invalid<cnv::greater, 1>(-alpha);
    invalid<cnv::greater, 1>(0.0);
    invalid<cnv::greater, 1>(1.0);
    constexpr_valid<cnv::greater, 1>(alpha);
  };

  test("greater_equal constrained_value") = [] {
    invalid<cnv::greater_equal, 1>(-alpha);
    invalid<cnv::greater_equal, 1>(0.0);
    constexpr_valid<cnv::greater_equal, 1>(1.0);
    constexpr_valid<cnv::greater_equal, 1>(alpha);
  };

  test("bounded constrained_value") = [] {
    invalid<cnv::bounded, -1, +1>(-alpha);
    invalid<cnv::bounded, -1, +1>(+alpha);

    constexpr_valid<cnv::bounded, -1, +1>(-1.0);
    constexpr_valid<cnv::bounded, -1, +1>(+1.0);

    constexpr_valid<cnv::bounded, -1, +1>(-0.0);
    constexpr_valid<cnv::bounded, -1, +1>(+0.0);
  };

  test("strictly_bounded constrained_value") = [] {
    invalid<cnv::strictly_bounded, -1, +1>(-alpha);
    invalid<cnv::strictly_bounded, -1, +1>(+alpha);

    invalid<cnv::strictly_bounded, -1, +1>(-1.0);
    invalid<cnv::strictly_bounded, -1, +1>(+1.0);

    constexpr_valid<cnv::strictly_bounded, -1, +1>(-0.0);
    constexpr_valid<cnv::strictly_bounded, -1, +1>(+0.0);
  };
}
