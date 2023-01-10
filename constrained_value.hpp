#pragma once

#include "src/constrained_value.hpp"
#include "src/predicate.hpp"

#include <concepts>

/// Define types with constraints
namespace constrained_value {

/// A value always less than zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T>
using negative = constrained_value<T, predicate::negative>;

/// A value always greater than or equal to zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T>
using nonnegative = constrained_value<T, predicate::nonnegative>;

/// A value always greater than zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T>
using positive = constrained_value<T, predicate::positive>;

/// A value always less than or equal to zero
/// @tparam T underlying type
/// @requires `T{}` must be the zero value
///
template <std::totally_ordered T>
using nonpositive = constrained_value<T, predicate::nonpositive>;

/// A value less than a supremum
/// @tparam T underlying type
/// @tparam sup
///
template <std::totally_ordered T, auto sup>
using less = constrained_value<T, predicate::less::bind_back<sup>>;

/// A value less than or equal to a maximum
/// @tparam T underlying type
/// @tparam max
///
template <std::totally_ordered T, auto max>
using less_equal = constrained_value<T, predicate::less_equal::bind_back<max>>;

/// A value greater than an infimum
/// @tparam T underlying type
/// @tparam inf
///
template <std::totally_ordered T, auto inf>
using greater = constrained_value<T, predicate::greater::bind_back<inf>>;

/// A value greater than or equal to a minumum
/// @tparam T underlying type
/// @tparam min
///
template <std::totally_ordered T, auto min>
using greater_equal =
    constrained_value<T, predicate::greater_equal::bind_back<min>>;

/// A value contained by lower and upper bounds
/// @tparam T underlying type
/// @tparam lo lower bound
/// @tparam hi upper bound
///
template <std::totally_ordered T, auto lo, auto hi>
using bounded =
    constrained_value<T,
                      predicate::greater_equal::bind_back<lo>,
                      predicate::less_equal::bind_back<hi>>;

/// A value strictly contained by lower and upper bounds
/// @tparam T underlying type
/// @tparam lo strict lower bound
/// @tparam hi strict upper bound
///
template <std::totally_ordered T, auto lo, auto hi>
using strictly_bounded =
    constrained_value<T,
                      predicate::greater::bind_back<lo>,
                      predicate::less::bind_back<hi>>;

/// A value with absolute value equal to one
/// @tparam T underlying type
///
template <typename T>
using unity = constrained_value<T, predicate::unity>;

}  // namespace constrained_value
