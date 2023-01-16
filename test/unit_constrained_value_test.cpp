#include "constrained_value/constrained_value.hpp"
#include "utility.hpp"

#include <boost/ut.hpp>

#include <complex>

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
}
