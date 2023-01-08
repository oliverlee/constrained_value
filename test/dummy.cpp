#include "constrained_value/constrained_value.hpp"

#include <boost/ut.hpp>

auto main() -> int
{
  using namespace boost::ut;

  test("dummy test") = [] { expect(0_i == 0); };
}
