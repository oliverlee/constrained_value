#include "constrained_value/constrained_value.hpp"
#include "utility.hpp"

#include <boost/ut.hpp>

#include <cmath>

// NOLINTBEGIN(readability-magic-numbers)

auto main() -> int
{
  namespace cnv = ::constrained_value;
  using namespace cnv::test;
  using namespace ::boost::ut;

  test("near constrained_value") = [] {
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

    using cnv::constant::_;

    constexpr_valid<cnv::near, _(1.0), _(0.1)>(0.9);
    constexpr_valid<cnv::near, _(1.0), _(0.1)>(1.0);
    constexpr_valid<cnv::near, _(1.0), _(0.1)>(1.1);

    invalid<cnv::near, _(1.0), _(0.1)>(std::nextafter(1.0 + 0.1, +2.0));
    invalid<cnv::near, _(1.0), _(0.1)>(std::nextafter(1.0 - 0.1, -2.0));
  };
}

// NOLINTEND(readability-magic-numbers)
