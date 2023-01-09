#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>
#include <complex>

namespace cnv = ::constrained_value;

template <typename T>
using right_half_plane =
    cnv::constrained_value<T,
                           decltype([](auto z) { return std::real(z) >= 0; })>;

template <class T>
constexpr auto norm(const T& z)
{
#ifdef __clang__
  // not yet constexpr
  return (std::real(z) * std::real(z)) +  //
         (std::imag(z) * std::imag(z));
#else
  return std::norm(z);
#endif
}

auto main() -> int
{
  using namespace ::boost::ut;

  test("implicitly converts to underlying type") = [] {
    using T = std::complex<double>;

    static constexpr auto z = right_half_plane<T>{T{3, 4}};

    // template argument deduction does not consider implicit conversions so we
    // specify a function to remove the need for argument deduction
    expect(constant<25.0_d == norm<T>(z)>);
  };

  test("never returns a mutable reference to underlying value") = [] {
    using T = std::complex<double>;
    using Z = right_half_plane<T>;

    static_assert(std::same_as<const T&, decltype(std::declval<Z&>().value())>);
    static_assert(
        std::same_as<const T&, decltype(std::declval<const Z&>().value())>);
    static_assert(std::same_as<T, decltype(std::declval<Z&&>().value())>);
    static_assert(std::same_as<T, decltype(std::declval<const Z&&>().value())>);
  };
}
