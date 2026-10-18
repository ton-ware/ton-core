#include <catch2/catch_test_macros.hpp>
#include <math/point2.hpp>
#include <type_traits>

TEST_CASE("Point2 memory layout and POD traits", "[ton][math][point2]")
{
    using namespace ton::math;
    using namespace ton::core;

    STATIC_REQUIRE(sizeof(Point2I16) == 4);
    STATIC_REQUIRE(sizeof(Point2I32) == 8);
    STATIC_REQUIRE(sizeof(Point2<F32>) == 8);
    STATIC_REQUIRE(sizeof(Point2<F64>) == 16);

    STATIC_REQUIRE(std::is_standard_layout_v<Point2I32>);
    STATIC_REQUIRE(std::is_trivially_copyable_v<Point2I32>);
    STATIC_REQUIRE(std::is_trivially_copyable_v<Point2I16>);
}

TEST_CASE("Point2 initialization and constexpr evaluation", "[ton][math][point2]")
{
    using namespace ton::math;

    constexpr Point2I32 default_origin{};
    STATIC_REQUIRE(default_origin.x == 0);
    STATIC_REQUIRE(default_origin.y == 0);

    constexpr Point2I32 custom_pt{10, -25};
    STATIC_REQUIRE(custom_pt.x == 10);
    STATIC_REQUIRE(custom_pt.y == -25);

    constexpr Point2I16 embedded_pt{-320, 240};
    STATIC_REQUIRE(embedded_pt.x == -320);
    STATIC_REQUIRE(embedded_pt.y == 240);
}

TEST_CASE("Point2 equality and relational comparisons", "[ton][math][point2]")
{
    using namespace ton::math;

    constexpr Point2I32 a{5, 10};
    constexpr Point2I32 b{5, 10};
    constexpr Point2I32 c{5, 12};
    constexpr Point2I32 d{3, 20};

    STATIC_REQUIRE(a == b);
    STATIC_REQUIRE(a != c);
    STATIC_REQUIRE(d < a); // Lexicographical comparison: d.x (3) < a.x (5)
    STATIC_REQUIRE(a < c); // a.x == c.x, but a.y (10) < c.y (12)
    STATIC_REQUIRE(c > a);
    STATIC_REQUIRE(a <= b);
    STATIC_REQUIRE(a >= b);
}

TEST_CASE("Point2 subscript operator access and mutation", "[ton][math][point2]")
{
    using namespace ton::math;

    Point2I32 p{100, 200};

    REQUIRE(p[0] == 100);
    REQUIRE(p[1] == 200);

    p[0] = 300;
    p[1] = 400;

    REQUIRE(p.x == 300);
    REQUIRE(p.y == 400);

    const Point2I32 const_p{-42, 84};
    REQUIRE(const_p[0] == -42);
    REQUIRE(const_p[1] == 84);
}

TEST_CASE("Point2 raw memory buffer access", "[ton][math][point2]")
{
    using namespace ton::math;

    const Point2I32 p{15, 30};
    const auto* buffer = p.data();

    REQUIRE(buffer != nullptr);
    REQUIRE(buffer[0] == 15);
    REQUIRE(buffer[1] == 30);
}
