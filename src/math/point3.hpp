#pragma once

#include <core/types.hpp>

namespace ton::math {
template <typename T = core::I32> struct Point3
{
    T x{};
    T y{};
    T z{};

    constexpr Point3() noexcept = default;
    constexpr Point3(T _x, T _y, T _z) noexcept : x(_x), y(_y), z(_z) {};

    [[nodiscard]] constexpr bool operator==(const Point3<T>& _p) const noexcept = default;
    [[nodiscard]] constexpr auto operator<=>(const Point3<T>& _p) const noexcept = default;
    [[nodiscard]] constexpr T& operator[](core::Usize _idx) noexcept
    {
        switch (_idx) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                return x;
        }
    }
    [[nodiscard]] constexpr const T& operator[](core::Usize _idx) const noexcept
    {
        switch (_idx) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                return x;
        }
    }
    [[nodiscard]] constexpr const T* data() const noexcept { return &x; }
};

using Point3I16 = Point3<core::I16>;
using Point3I32 = Point3<core::I32>;
using Point3U32 = Point3<core::U32>;
using Point3F32 = Point3<core::F32>;
} // namespace ton::math
