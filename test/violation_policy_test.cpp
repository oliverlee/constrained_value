#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>

namespace cnv = ::constrained_value;

struct nonpositive_value_error
{};

using positive_double = cnv::constrained_value<
    double,                    //
    cnv::predicate::positive,  //
    decltype([](auto&&...) { throw nonpositive_value_error{}; })>;

auto main() -> int
{
  using namespace ::boost::ut;

  test("constrained value is constructible") = [] {
    static constexpr auto x = positive_double{1.0};

    static_assert(1.0_d == x);
  };

  test("throws when constructing with an invalid value") = [] {
    expect(
        throws<nonpositive_value_error>([] { (void)(positive_double{0.0}); }));
  };
}
