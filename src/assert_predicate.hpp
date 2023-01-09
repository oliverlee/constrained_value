#pragma once

#include "src/source_location.hpp"

#include <concepts>
#include <functional>

namespace constrained_value {

/// Checks if a predicate is satisfied for a value and calls a handler if not
/// @tparam V violation policy
/// @param value value of a type
/// @param predicate function representing a type invariant
/// @param caller function verifying the type invariant
/// @param source_location source location invoking caller
///
template <typename V, typename T, std::predicate<T> P>
constexpr auto assert_predicate(
    const T& value, const P& predicate, const char* caller, source_location sl)
    -> void
{
  if (std::invoke(predicate, value)) {
    return;
  }

  std::invoke(V{}, value, predicate, caller, sl);
}

}  // namespace constrained_value
