#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>

#include <complex>
#include <tuple>

auto main() -> int
{
  namespace cnv = ::constrained_value;
  using namespace ::boost::ut;

  constexpr auto numeric_types = std::tuple<
      bool,
      char,
      signed char,
      unsigned char,
      wchar_t,
      char8_t,
      char16_t,
      char32_t,
      short,
      unsigned short,
      int,
      unsigned int,
      long,
      unsigned long,
      long long,
      unsigned long long,
      float,
      double,
      long double,
      cnv::constant::bitwise<double>>{};

  test("types implement numeric_limits") = []<typename T>() {
    static_assert(cnv::math::numeric_limits<T>);
  } | numeric_types;

  test("types model numeric") = []<typename T>() {
    static_assert(cnv::math::numeric<T>);
  } | numeric_types;

  constexpr auto non_numeric_types =
      std::tuple<decltype([] {}), std::tuple<>, std::complex<double>>{};

  test("types do not implement numeric_limits") = []<typename T>() {
    static_assert(not cnv::math::numeric_limits<T>);
  } | non_numeric_types;

  test("types do not model numeric") = []<typename T>() {
    static_assert(not cnv::math::numeric<T>);
  } | non_numeric_types;

  test("constant::bitwise<T> doesn't specialize numeric limits if T doesn't") =
      [] {
        static_assert(
            not std::numeric_limits<std::complex<double>>::is_specialized);
        static_assert(
            not std::numeric_limits<
                cnv::constant::bitwise<std::complex<double>>>::is_specialized);
      };
}
