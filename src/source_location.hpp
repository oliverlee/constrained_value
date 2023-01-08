#pragma once

#if __has_include(<source_location>)
#include <source_location>
#endif

namespace constrained_value {

#if defined(__cpp_lib_source_location)
using source_location = std::source_location;
#else
// https://clang.llvm.org/docs/LanguageExtensions.html#source-location-builtins
class source_location
{
  unsigned line_;
  unsigned column_;
  const char* file_;
  const char* function_;

  source_location() = default;

public:
  // NOLINTBEGIN(bugprone-easily-swappable-parameters)
  [[nodiscard]] static constexpr auto
  current(unsigned line = __builtin_LINE(),
          unsigned column = __builtin_COLUMN(),
          const char* file = __builtin_FILE(),
          const char* function = __builtin_FUNCTION()) noexcept
  // NOLINTEND(bugprone-easily-swappable-parameters)
  {
    auto sl = source_location{};

    sl.line_ = line;
    sl.column_ = column;
    sl.file_ = file;
    sl.function_ = function;

    return sl;
  }

  [[nodiscard]] constexpr auto line() const noexcept { return line_; }
  [[nodiscard]] constexpr auto column() const noexcept { return column_; }
  [[nodiscard]] constexpr auto file_name() const noexcept { return file_; }
  [[nodiscard]] constexpr auto function_name() const noexcept
  {
    return function_;
  }
};
#endif

}  // namespace constrained_value
