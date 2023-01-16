#pragma once

#include <bit>
#include <concepts>
#include <cstdint>

namespace constrained_value {
namespace detail {

template <typename T>
consteval auto signed_integer_for() noexcept
{
  if constexpr (sizeof(T) == sizeof(std::int64_t)) {
    return std::int64_t{};
  } else if constexpr (sizeof(T) == sizeof(std::int32_t)) {
    return std::int32_t{};
  } else if constexpr (sizeof(T) == sizeof(std::int16_t)) {
    return std::int16_t{};
  } else if constexpr (sizeof(T) == sizeof(std::int8_t)) {
    return std::int8_t{};
  }
}

}  // namespace detail

/// Bit manipulation functions
///
/// @{

/// Signed integer type with the same size as `T`
///
template <typename T>
using bitwise_integer_for_t = decltype(detail::signed_integer_for<T>());

// clang-format off

/// Checks if a type can be `bit_cast` into a fixed-width signed integer 
///
template <typename T>
concept bitwise_integer_reinterpretable =
  requires(T t) {
    std::bit_cast<bitwise_integer_for_t<T>>(t);
  };

// clang-format on

/// Bit cast a value into a corresponding signed integer
///
template <bitwise_integer_reinterpretable T>
constexpr auto bitwise_integer_reinterpretation(const T& value) noexcept
{
  return std::bit_cast<bitwise_integer_for_t<T>>(value);
}

/// @}

}  // namespace constrained_value
