#pragma once

namespace constrained_value::detail {

template <int N>
struct priority : priority<N - 1>
{};

template <>
struct priority<0>
{};

}  // namespace constrained_value::detail
