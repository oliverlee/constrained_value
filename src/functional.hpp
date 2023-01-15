#pragma once

#include <concepts>
#include <utility>

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace constrained_value {

/// Higher order function objects
///
namespace functional {

/// Logical AND of multiple function objects
template <std::default_initializable... Fs>
struct all_of
{
  template <typename... Args>
  constexpr auto operator()(Args&&... args) const
      noexcept(noexcept(((Fs{}(std::forward<Args>(args)...)) and ...)))
          -> decltype(((Fs{}(std::forward<Args>(args)...)) and ...))
  {
    return ((Fs{}(std::forward<Args>(args)...)) and ...);
  }
};

/// Compose a set of function objects
///
/// Invokes each function object with the return value of the one before.
///
///~~~{.cpp}
/// assert(compose<F, G>(x) == G{}(F{}(x)));
///~~~
///
/// @{
template <std::default_initializable... Fs>
struct compose;

template <std::default_initializable F, std::default_initializable G>
struct compose<F, G>
{
  template <typename T>
  constexpr auto operator()(T&& value) const
      noexcept(noexcept(G{}(F{}(std::forward<T>(value)))))
          -> decltype(G{}(F{}(std::forward<T>(value))))
  {
    return G{}(F{}(std::forward<T>(value)));
  }
};

template <std::default_initializable F, std::default_initializable... Gs>
  requires(sizeof...(Gs) != 0)
struct compose<F, Gs...>
{
  using G = compose<Gs...>;

  template <typename T>
  constexpr auto operator()(T&& value) const
      noexcept(noexcept(G{}(F{}(std::forward<T>(value)))))
          -> decltype(G{}(F{}(std::forward<T>(value))))
  {
    return G{}(F{}(std::forward<T>(value)));
  }
};
/// @}

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
    template <typename... Head>
    constexpr auto operator()(Head&&... head) const noexcept(
        noexcept(std::invoke(F{}, std::forward<Head>(head)..., tail...)))
        -> decltype(std::invoke(F{}, std::forward<Head>(head)..., tail...))
    {
      return std::invoke(F{}, std::forward<Head>(head)..., tail...);
    }
  };
};

}  // namespace functional
}  // namespace constrained_value
