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
template <typename V, typename T, std::predicate<T> P>
  requires violation_policy<V, T, P, source_location>
constexpr auto assert_predicate(
    const T& value,
    const P& predicate,
    const char* caller,
    const source_location& sl)                            //
    noexcept(noexcept(std::invoke(predicate, value)) and  //
             noexcept(std::invoke(V{}, value, predicate, caller, sl))) -> bool
{
  if (std::invoke(predicate, value)) {
    return true;
  }

  std::invoke(V{}, value, predicate, caller, sl);
  return false;
}

template <typename V, typename... Ps, typename T>
constexpr auto
assert_predicates(const T& value,
                  const char* caller = "",
                  const source_location& sl = source_location::current())  //
    noexcept(noexcept((assert_predicate<V>(value, Ps{}, caller, sl) and ...)))
        -> bool
{
  return (assert_predicate<V>(value, Ps{}, caller, sl) and ...);
}

}  // namespace constrained_value
