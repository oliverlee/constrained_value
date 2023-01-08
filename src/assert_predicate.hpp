#pragma once

#include "src/detail/type_name.hpp"
#include "src/source_location.hpp"

#include <concepts>
#include <cstdlib>
#include <functional>
#include <iostream>

namespace constrained_value {

template <typename T, std::predicate<T> P>
constexpr auto assert_predicate(
    const T& value, const P& predicate, const char* caller, source_location sl)
    -> void
{
  if (std::invoke(predicate, value)) {
    return;
  }

  std::cerr << "file: " << sl.file_name() << "(" << sl.line() << ":"
            << sl.column() << ") `" << sl.function_name() << "`: "
            << "contract violated in `" << caller << "`. "
            << detail::type_name<P>() << "(" << value << ") is false.\n";

  std::abort();
}

}  // namespace constrained_value
