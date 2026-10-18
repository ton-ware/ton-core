#include <catch2/catch_test_macros.hpp>

TEST_CASE("TonCore test harness sanity check", "[core][sanity]")
{
    STATIC_REQUIRE(sizeof(float) == 4);
    STATIC_REQUIRE(sizeof(double) == 8);
}
