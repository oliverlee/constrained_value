#pragma once

#include <string_view>

namespace constrained_value::detail {

// https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/56766138#56766138
template <typename T>
[[nodiscard]] consteval auto type_name() noexcept
{
  constexpr auto prefix = std::string_view{
#ifdef __clang__
      "auto constrained_value::detail::type_name() [T = "
#elif defined(__GNUC__)
      "consteval auto constrained_value::detail::type_name() [with T = "
#endif
  };

  constexpr auto suffix = std::string_view{"]"};

  auto name = std::string_view{__PRETTY_FUNCTION__};
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}

}  // namespace constrained_value::detail
