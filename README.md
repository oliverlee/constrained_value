# constrained_value

A library that embeds constraints into types.

## examples

Specify function preconditions.

```cpp
auto valid_sqrt(cnv::nonnegative<float> arg)
{
  return std::sqrt<float>(arg);
}

auto x = valid_sqrt(-1.0F); // assertion failure
```

Specify types with invariants.

```cpp
struct options
{
  cnv::unity<std::complex<double>> rotation;
  cnv::bounded<double, 0, 1>       reduction_factor;
};
```

Specify a type with a specific invariant violation policy and arbitrary
invariants.

```cpp
struct invalid_value_error
  : std::exception
{};

using unit_right_half_plane =
  cnv::constrained_value<
    std::complex<double>,
    cnv::functional::all_of<
      decltype([](auto z) { return norm(z) == 1; }),
      decltype([](auto z) { return real(z) > 0; })
    >,
    decltype([](auto&&...) { throw invalid_value_error{}; })
  >;
```
