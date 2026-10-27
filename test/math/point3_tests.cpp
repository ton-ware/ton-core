#include <catch2/catch_test_macros.hpp>
#include <math/point3.hpp>
#include <type_traits>

TEST_CASE("Point3 memory layout and POD traits", "[ton][math][point3]")
{
    using namespace ton::math;
    using namespace ton::core;

    STATIC_REQUIRE(sizeof(Point3I16) == 6);
    STATIC_REQUIRE(sizeof(Point3I32) == 12);
    STATIC_REQUIRE(sizeof(Point3U32) == 12);
    STATIC_REQUIRE(sizeof(Point3<F32>) == 12);
    STATIC_REQUIRE(sizeof(Point3<F64>) == 24);

    STATIC_REQUIRE(std::is_standard_layout_v<Point3I32>);
    STATIC_REQUIRE(std::is_trivially_copyable_v<Point3I32>);
    STATIC_REQUIRE(std::is_trivially_copyable_v<Point3I16>);
}

TEST_CASE("Point3 initialization and constexpr evaluation", "[ton][math][point3]")
{
    using namespace ton::math;

    constexpr Point3I32 default_origin{};
    STATIC_REQUIRE(default_origin.x == 0);
    STATIC_REQUIRE(default_origin.y == 0);
    STATIC_REQUIRE(default_origin.z == 0);

    constexpr Point3I32 custom_pt{10, -25, 42};
    STATIC_REQUIRE(custom_pt.x == 10);
    STATIC_REQUIRE(custom_pt.y == -25);
    STATIC_REQUIRE(custom_pt.z == 42);

    constexpr Point3I16 embedded_pt{-320, 240, 1024};
    STATIC_REQUIRE(embedded_pt.x == -320);
    STATIC_REQUIRE(embedded_pt.y == 240);
    STATIC_REQUIRE(embedded_pt.z == 1024);
}

TEST_CASE("Point3 equality and relational comparisons", "[ton][math][point3]")
{
    using namespace ton::math;

    constexpr Point3I32 a{5, 10, 15};
    constexpr Point3I32 b{5, 10, 15};
    constexpr Point3I32 c{5, 10, 20};
    constexpr Point3I32 d{5, 12, 1};
    constexpr Point3I32 e{3, 20, 100};

    STATIC_REQUIRE(a == b);
    STATIC_REQUIRE(a != c);
    STATIC_REQUIRE(e < a); // Lexicographical: e.x (3) < a.x (5)
    STATIC_REQUIRE(a < d); // a.x == d.x, a.y (10) < d.y (12)
    STATIC_REQUIRE(a < c); // a.x == c.x, a.y == c.y, a.z (15) < c.z (20)
    STATIC_REQUIRE(c > a);
    STATIC_REQUIRE(a <= b);
    STATIC_REQUIRE(a >= b);
}

TEST_CASE("Point3 subscript operator access and mutation", "[ton][math][point3]")
{
    using namespace ton::math;

    Point3I32 p{100, 200, 300};

    REQUIRE(p[0] == 100);
    REQUIRE(p[1] == 200);
    REQUIRE(p[2] == 300);

    p[0] = 400;
    p[1] = 500;
    p[2] = 600;

    REQUIRE(p.x == 400);
    REQUIRE(p.y == 500);
    REQUIRE(p.z == 600);

    constexpr Point3I32 const_p{-42, 84, -126};
    STATIC_REQUIRE(const_p[0] == -42);
    STATIC_REQUIRE(const_p[1] == 84);
    STATIC_REQUIRE(const_p[2] == -126);
}

TEST_CASE("Point3 raw memory buffer access", "[ton][math][point3]")
{
    using namespace ton::math;

    const Point3I32 p{15, 30, 45};
    const auto* buffer = p.data();

    REQUIRE(buffer != nullptr);
    REQUIRE(buffer[0] == 15);
    REQUIRE(buffer[1] == 30);
    REQUIRE(buffer[2] == 45);
}
