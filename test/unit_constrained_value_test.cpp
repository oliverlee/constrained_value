#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>
#include <complex>

namespace cnv = ::constrained_value;

template <template <typename> typename constraint, typename T>
consteval auto constexpr_valid(T value)
{
  (void)constraint<T>{value};
}

template <template <typename> typename constraint, typename T>
auto valid(T value)
{
  (void)constraint<T>{value};
}

template <template <typename> typename constraint, typename T>
auto invalid(T value)
{
  using ::boost::ut::aborts;
  using ::boost::ut::expect;

  expect(aborts([&] { (void)constraint<T>{value}; }));
}

auto main() -> int
{
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
