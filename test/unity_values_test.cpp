#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>
#include <complex>

namespace cnv = ::constrained_value;

auto main() -> int
{
  using namespace ::boost::ut;

  test("unity constrained_value") = [] {
    constexpr auto all_valid = std::tuple{
        cnv::unity<int>{+1},
        cnv::unity<int>{-1},
        cnv::unity<double>{+1.0},
        cnv::unity<double>{-1.0},
        cnv::unity<std::complex<double>>{std::complex{+1.0, 0.0}},
        cnv::unity<std::complex<double>>{std::complex{-1.0, 0.0}},
        cnv::unity<std::complex<double>>{std::complex{0.0, +1.0}},
        cnv::unity<std::complex<double>>{std::complex{0.0, -1.0}}};

    (void)all_valid;

    expect(aborts([] { (void)cnv::unity<int>{}; }));
    expect(aborts([] { (void)cnv::unity<double>{}; }));
    expect(aborts([] { (void)cnv::unity<std::complex<double>>{}; }));
  };
}
