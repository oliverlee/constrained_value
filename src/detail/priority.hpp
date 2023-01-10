#pragma once

namespace constrained_value {
namespace detail {

template <int N>
struct priority : priority<N - 1>
{};
template <>
struct priority<0>
{};

}  // namespace detail
}  // namespace constrained_value
