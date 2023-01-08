#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>
#include <tuple>

namespace cnv = ::constrained_value;

template <typename T, auto n>
using greater_than =
    cnv::constrained_value<T, cnv::predicate::greater::bind_back<n>>;

template <class T>
struct single_value
{
  cnv::positive<T> value;
};

constexpr auto test_types = std::tuple<float, double>{};

auto main() -> int
{
  using namespace ::boost::ut;

  test("constructible from underlying type") = []<typename T> {
    static constexpr auto x = cnv::positive<T>{T{1}};
    static constexpr auto y = greater_than<T, 1>{T{2}};
    static constexpr auto z = single_value<T>{.value = {T{1}}};

    (void)x;
    (void)y;
    (void)z;
  } | test_types;

  test("not constructible for non-underlying type") = []<typename T> {
    static_assert(not std::constructible_from<cnv::positive<T>, int>);
    static_assert(not std::constructible_from<greater_than<T, 1>, int>);
  } | test_types;

  test("aborts for invalid value") = []<typename T> {
    expect(aborts([] { (void)cnv::positive<T>{T{}}; }));
    expect(aborts([] { (void)greater_than<T, 1>{T{}}; }));
  } | test_types;
}
