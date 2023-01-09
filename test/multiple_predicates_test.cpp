#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>

// NOLINTBEGIN(readability-magic-numbers)

namespace cnv = ::constrained_value;

auto main() -> int
{
  using namespace ::boost::ut;

  test("constrained value with multiple predicates is constructible") = [] {
    static constexpr auto x = cnv::strictly_bounded<double, 0, 1>{0.1};

    static_assert(0.1_d == x);
  };

  test("aborts when value exceeds upper bound") = [] {
    expect(aborts([] { (void)(cnv::strictly_bounded<double, 0, 1>{1.0}); }));
    expect(aborts([] { (void)(cnv::strictly_bounded<double, 0, 1>{2.0}); }));
  };

  test("aborts when value subceeds lower bound") = [] {
    expect(aborts([] { (void)(cnv::strictly_bounded<double, 0, 1>{0.0}); }));
    expect(aborts([] { (void)(cnv::strictly_bounded<double, 0, 1>{-1.0}); }));
  };
}

// NOLINTEND(readability-magic-numbers)
