#pragma once

#include <concepts>
#include <utility>

namespace constrained_value::functional {

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

}  // namespace constrained_value::functional
