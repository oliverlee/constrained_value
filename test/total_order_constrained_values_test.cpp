#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>
#include <cmath>

namespace cnv = ::constrained_value;

// positive test constant
template <typename T>
static constexpr auto alpha = T{10};

auto main() -> int
{
  using namespace ::boost::ut;

  // clang-format off
  test("negative constrained_value") = [] {
    expect(constant<-alpha<double> == cnv::negative<double>{-alpha<double>}>);
    expect(aborts([] { cnv::negative<double>{-double{}}; }));
    expect(aborts([] { cnv::negative<double>{+double{}}; }));
    expect(aborts([] { cnv::negative<double>{+alpha<double>}; }));
  };

  test("nonnegative constrained_value") = [] {
    expect(aborts([] { cnv::nonnegative<double>{-alpha<double>}; }));
    expect(constant<-double{} == cnv::nonnegative<double>{-double{}}>);
    expect(constant<+double{} == cnv::nonnegative<double>{+double{}}>);
    expect(constant<+alpha<double> == cnv::nonnegative<double>{+alpha<double>}>);
  };

  test("positive constrained_value") = [] {
    expect(aborts([] { cnv::positive<double>{-alpha<double>}; }));
    expect(aborts([] { cnv::positive<double>{-double{}}; }));
    expect(aborts([] { cnv::positive<double>{+double{}}; }));
    expect(constant<+alpha<double> == cnv::positive<double>{+alpha<double>}>);
  };

  test("nonpositive constrained_value") = [] {
    expect(constant<-alpha<double> == cnv::nonpositive<double>{-alpha<double>}>);
    expect(constant<-double{} == cnv::nonpositive<double>{-double{}}>);
    expect(constant<+double{} == cnv::nonpositive<double>{+double{}}>);
    expect(aborts([] { cnv::nonpositive<double>{+alpha<double>}; }));
  };

  test("equal to constrained_value") = [] {
    expect(aborts([] { cnv::equal_to<double, 0>{-double{1}}; }));
    expect(constant<double{} == cnv::equal_to<double, 0>{double{}}>);
    expect(constant<double{} == cnv::equal_to<double, cnv::constant::Zero{}>{double{}}>);
    expect(aborts([] { cnv::equal_to<double, 0>{+double{1}}; }));

    expect(aborts([] { cnv::equal_to<double, 0>{std::nextafter(double{}, +1.0)}; }));
    expect(aborts([] { cnv::equal_to<double, 0>{std::nextafter(double{}, -1.0)}; }));
  };

  test("not equal to constrained_value") = [] {
    expect(constant<-double{1} == cnv::not_equal_to<double, 0>{-double{1}}>);
    expect(aborts([] { cnv::not_equal_to<double, 0>{double{}}; }));
    expect(aborts([] { cnv::not_equal_to<double, cnv::constant::Zero{}>{double{}}; }));
    expect(constant<+double{1} == cnv::not_equal_to<double, 0>{+double{1}}>);

    expect(0.0_d < cnv::not_equal_to<double, 0>{std::nextafter(double{}, +1.0)});
    expect(0.0_d > cnv::not_equal_to<double, 0>{std::nextafter(double{}, -1.0)});
  };

  test("less constrained_value") = [] {
    expect(constant<-alpha<double> == cnv::less<double, 1>{-alpha<double>}>);
    expect(constant<-double{} == cnv::less<double, 1>{-double{}}>);
    expect(constant<+double{} == cnv::less<double, 1>{+double{}}>);
    expect(aborts([] { cnv::less<double, 1>{+double{1}}; }));
    expect(aborts([] { cnv::less<double, 1>{+alpha<double>}; }));
  };

  test("less_equal constrained_value") = [] {
    expect(constant<-alpha<double> == cnv::less_equal<double, 1>{-alpha<double>}>);
    expect(constant<-double{} == cnv::less_equal<double, 1>{-double{}}>);
    expect(constant<+double{} == cnv::less_equal<double, 1>{+double{}}>);
    expect(constant<+double{1} == cnv::less_equal<double, 1>{+double{1}}>);
    expect(aborts([] { cnv::less_equal<double, 1>{+alpha<double>}; }));
  };

  test("greater constrained_value") = [] {
    expect(aborts([] { cnv::greater<double, -1>{-alpha<double>}; }));
    expect(aborts([] { cnv::greater<double, -1>{-double{1}}; }));
    expect(constant<-double{} == cnv::greater<double, -1>{-double{}}>);
    expect(constant<+double{} == cnv::greater<double, -1>{+double{}}>);
    expect(constant<+alpha<double> == cnv::greater<double, -1>{+alpha<double>}>);
  };

  test("greater_equal constrained_value") = [] {
    expect(aborts([] { cnv::greater_equal<double, -1>{-alpha<double>}; }));
    expect(constant<-double{1} == cnv::greater_equal<double, -1>{-double{1}}>);
    expect(constant<-double{} == cnv::greater_equal<double, -1>{-double{}}>);
    expect(constant<+double{} == cnv::greater_equal<double, -1>{+double{}}>);
    expect(constant<+alpha<double> == cnv::greater_equal<double, -1>{+alpha<double>}>);
  };

  test("bounded constrained_value") = [] {
    expect(aborts([] { cnv::bounded<double, -1, +1>{-alpha<double>}; }));
    expect(constant<-double{1} == cnv::bounded<double, -1, +1>{-double{1}}>);
    expect(constant<-double{} == cnv::bounded<double, -1, +1>{-double{}}>);
    expect(constant<+double{} == cnv::bounded<double, -1, +1>{+double{}}>);
    expect(constant<+double{1} == cnv::bounded<double, -1, +1>{+double{1}}>);
    expect(aborts([] { cnv::bounded<double, -1, +1>{+alpha<double>}; }));
  };

  test("strictly_bounded constrained_value") = [] {
    expect(aborts([] { cnv::strictly_bounded<double, -1, +1>{-alpha<double>}; }));
    expect(aborts([] { cnv::strictly_bounded<double, -1, +1>{-double{1}}; }));
    expect(constant<-double{} == cnv::strictly_bounded<double, -1, +1>{-double{}}>);
    expect(constant<+double{} == cnv::strictly_bounded<double, -1, +1>{+double{}}>);
    expect(aborts([] { cnv::strictly_bounded<double, -1, +1>{+double{1}}; }));
    expect(aborts([] { cnv::strictly_bounded<double, -1, +1>{+alpha<double>}; }));
  };
  // clang-format on
}
