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

    //(void)cnv::unit<double, 0.1>{+1.1};
    (void)cnv::unit<double, cnv::constant::ulp<4>>{+1.0};

    (void)cnv::unit<std::complex<double>, cnv::constant::ulp<4>>{std::complex{1.0, 0.0}};
    (void)cnv::unit<std::complex<double> >{std::complex{1.0, 0.0}};

    valid<cnv::unit, cnv::constant::ulp<2>>(std::complex{-1.0, 0.0});

    valid<cnv::unit>(std::complex{+1.0, 0.0});
    valid<cnv::unit>(std::complex{0.0, -1.0});
    valid<cnv::unit>(std::complex{0.0, +1.0});

    invalid<cnv::unit>(0);
    invalid<cnv::unit>(0.0);
    invalid<cnv::unit>(std::complex<double>{});
  };
}
