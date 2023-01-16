#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>

#include <limits>

namespace cnv = ::constrained_value;
using namespace ::cnv::constant;

auto main() -> int
{
  using namespace ::boost::ut;

  test("double and constant::ulp on positive side") = [] {
    using Double = std::numeric_limits<double>;
    using cnv::constant::ulp;

    expect(0_i == cnv::ulp_distance(+0.0, +0.0 + ulp<0>));
    expect(0_i == cnv::ulp_distance(+0.0, +0.0 - ulp<0>));

    expect(2_i == cnv::ulp_distance(+0.0, +0.0 + ulp<2>));

    expect(2_i == cnv::ulp_distance(+1.0, +1.0 + ulp<2>));
    expect(-2_i == cnv::ulp_distance(+1.0, +1.0 - ulp<2>));

    expect(0_i == cnv::ulp_distance(Double::max(), Double::max() + ulp<0>));
    expect(0_i == cnv::ulp_distance(Double::max(), Double::max() - ulp<0>));
    expect(-1_i == cnv::ulp_distance(Double::max(), Double::max() - ulp<1>));

    expect(0_i ==
           cnv::ulp_distance(Double::max(), (Double::max() - ulp<1>)+ulp<1>));
  };

  test("double and constant::ulp on negative side") = [] {
    using Double = std::numeric_limits<double>;
    using cnv::constant::ulp;

    expect(0_i == cnv::ulp_distance(-0.0, -0.0 - ulp<0>));
    expect(0_i == cnv::ulp_distance(-0.0, -0.0 + ulp<0>));
    expect(-1_i == cnv::ulp_distance(-0.0, -0.0 - ulp<1>));
    expect(-1_i == cnv::ulp_distance(-0.0, -0.0 + ulp<-1>));

    expect(
        0_i == cnv::ulp_distance(Double::lowest(), Double::lowest() - ulp<0>));
    expect(
        0_i == cnv::ulp_distance(Double::lowest(), Double::lowest() + ulp<0>));
    expect(
        1_i == cnv::ulp_distance(Double::lowest(), Double::lowest() + ulp<1>));
    expect(
        1_i == cnv::ulp_distance(Double::lowest(), Double::lowest() - ulp<-1>));
  };

  test("double and constant::ulp changes sign") = [] {
    using Int = std::numeric_limits<int>;
    using Double = std::numeric_limits<double>;
    using cnv::constant::ulp;

    expect(-1_i == cnv::ulp_distance(+0.0, +0.0 - ulp<1>));
    expect(-1_i == cnv::ulp_distance(+0.0, (+0.0 + ulp<1>)-ulp<2>));

    expect(1_i == cnv::ulp_distance(-0.0, -0.0 + ulp<1>));
    expect(1_i == cnv::ulp_distance(-0.0, (-0.0 - ulp<1>)+ulp<2>));

    expect(Int::max() == cnv::ulp_distance(+0.0, +0.0 + ulp<Int::max()>));
    expect(Int::max() == cnv::ulp_distance(-0.0, -0.0 + ulp<Int::max()>));
    expect(-Int::max() == cnv::ulp_distance(+0.0, +0.0 - ulp<Int::max()>));
    expect(-Int::max() == cnv::ulp_distance(-0.0, -0.0 - ulp<Int::max()>));

    expect(Double::epsilon() > -0.0 + ulp<Int::max()>);
  };

  // TODO test against bitwise<double>
}
