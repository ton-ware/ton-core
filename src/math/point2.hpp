#pragma once

#include <core/types.hpp>

namespace ton::math {
template <typename T = ton::core::I32> struct Point2
{
    T x{};
    T y{};

    constexpr Point2() noexcept = default;
    constexpr Point2(T _x, T _y) noexcept : x(_x), y(_y) {};

    [[nodiscard]] constexpr bool operator==(const Point2<T>& _p) const noexcept = default;
    [[nodiscard]] constexpr auto operator<=>(const Point2<T>& _p) const noexcept = default;
    [[nodiscard]] constexpr T& operator[](ton::core::Usize _idx) noexcept
    {

        return (_idx == 0) ? x : y;
    }
    [[nodiscard]] constexpr const T& operator[](ton::core::Usize _idx) const noexcept
    {
        return (_idx == 0) ? x : y;
    }
    [[nodiscard]] constexpr const T* data() const noexcept { return &x; }
};

using Point2I16 = Point2<ton::core::I16>;
using Point2I32 = Point2<ton::core::I32>;
} // namespace ton::math
