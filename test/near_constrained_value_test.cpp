#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>
#include <cmath>

// NOLINTBEGIN(readability-magic-numbers)

namespace cnv = ::constrained_value;

auto main() -> int
{
  using namespace ::boost::ut;

  test("near constrained_value") = [] {
    (void)cnv::near<double, 0, 1>{+0.0};
    (void)cnv::near<double, 0, 1>{-0.0};

    (void)cnv::near<double, 0, 1>{+1.0};
    (void)cnv::near<double, 0, 1>{-1.0};

    (void)cnv::near<double, 0, 1>{+0.1};
    (void)cnv::near<double, 0, 1>{-0.1};

    expect(aborts([] {
      (void)cnv::near<double, 0, 1>{std::nextafter(1.0, 2.0)};
    }));
    expect(aborts([] {
      (void)cnv::near<double, 0, 1>{std::nextafter(-1.0, -2.0)};
    }));

    expect(aborts([] { (void)cnv::near<double, 0, 1>{-2.0}; }));
    expect(aborts([] { (void)cnv::near<double, 0, 1>{+2.0}; }));
  };
}

// NOLINTEND(readability-magic-numbers)
