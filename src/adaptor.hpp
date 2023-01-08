#pragma once

#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace constrained_value {

/// Function object adaptors
///
/// These adaptors wrap a function object and add additional functionality.
///
namespace adaptor {

/// Binds non-type template parameters to a function object
/// @tparam F function object
///
/// Function object adaptor that allows partial application of non-type template
/// parameters to the underlying function object `F`. This is used to define
/// `constrained_value` types with the same size as their underlying types.
///
template <std::default_initializable F>
struct nttp_bindable : F
{
  /// Partially applies args to `F`
  /// @tparam tail args bound to the end of an invocation of `F`
  ///
  template <auto... tail>
  struct bind_back
  {
    /// Invoke the wrapped function object
    /// @tparam Head head args
    /// @param head args applied to the front of an invocation of `F`
    ///
    /// Invokes `F` with `head` and `tail` args. Equivalent to
    /// ~~~{.cpp}
    /// std::invoke(F{}, head..., tail...)
    /// ~~~
    ///
    template <class... Head>
    constexpr auto operator()(Head&&... head) const noexcept(
        noexcept(std::invoke(F{}, std::forward<Head>(head)..., tail...)))
        -> decltype(std::invoke(F{}, std::forward<Head>(head)..., tail...))
    {
      return std::invoke(F{}, std::forward<Head>(head)..., tail...);
    }
  };
};

}  // namespace adaptor
}  // namespace constrained_value
