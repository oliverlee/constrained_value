#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>

namespace cnv = ::constrained_value;

auto main() -> int
{
  using namespace ::boost::ut;

  test("constrained value is assignable") = [] {
    static constexpr auto x = (cnv::positive<double>{1.0} = 2.0);

    static_assert(2.0_d == x);
  };

  test("aborts when assigning an invalid value") = [] {
    expect(aborts([] { (void)(cnv::positive<double>{1.0} = 0.0); }));
  };
}
