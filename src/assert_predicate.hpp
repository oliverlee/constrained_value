#pragma once

#include "src/source_location.hpp"
#include "src/violation_policy.hpp"

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
template <typename P, typename V, typename T>
  requires (std::predicate<P, T> and violation_policy<V, T, P, source_location>)
constexpr auto assert_predicate(
    const T& value,
    const char* caller,
    const source_location& sl)                              //
    noexcept(                                               //
        noexcept(std::invoke(P{}, value)) and               //
        noexcept(std::invoke(V{}, value, P{}, caller, sl))  //
        )                                                   //
    -> bool
{
  if (std::invoke(P{}, value)) {
    return true;
  }

  std::invoke(V{}, value, P{}, caller, sl);
  return false;
}

}  // namespace constrained_value
