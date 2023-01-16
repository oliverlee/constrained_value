#pragma once

#include "src/detail/type_name.hpp"
#include "src/source_location.hpp"

#include <concepts>
#include <cstdlib>
#include <iostream>
#include <type_traits>

namespace constrained_value {

/// Specifies a violation policy that is invoked on an invariant violation
/// @tparam V violation policy
/// @tparam T type with invariant
/// @tparam P invariant predicate
/// @tparam S type same as or similar to std::source_location
///
template <typename V, typename T, typename P, typename S>
concept violation_policy =
    std::default_initializable<V> and
    std::regular_invocable<
        const V,
        const T&,
        const P&,
        const char*,
        const S&> and
    std::is_void_v<
        std::invoke_result_t<V, const T&, const P&, const char*, const S&>>;

/// Predefined violation policies
struct on_violation
{
  /// Violation policy that prints an informational message and then aborts
  ///
  struct print_and_abort
  {
    template <typename T, typename P, typename SourceLocation>
    auto operator()(
        const T& value, const P&, const char* caller, const SourceLocation& sl)
        const -> void
    {
      std::cerr
          << "file: " << sl.file_name() << "(" << sl.line() << ":"
          << sl.column() << ") `" << sl.function_name() << "`: "
          << "contract violated in `" << caller << "`. "
          << detail::type_name<P>() << "(" << value << ") is false.\n";

      std::abort();
    }
  };
};

}  // namespace constrained_value
