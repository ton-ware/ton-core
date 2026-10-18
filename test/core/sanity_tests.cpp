#include <catch2/catch_test_macros.hpp>
#include <core/consts.hpp>
#include <core/types.hpp>
#include <type_traits>

TEST_CASE("TonCore test harness sanity check", "[core][sanity]")
{
    STATIC_REQUIRE(sizeof(float) == 4);
    STATIC_REQUIRE(sizeof(double) == 8);
}

TEST_CASE("TonCore primitive types and sizing", "[core][types]")
{
    using namespace ton::core;

    STATIC_REQUIRE(sizeof(I8) == 1);
    STATIC_REQUIRE(sizeof(U8) == 1);
    STATIC_REQUIRE(sizeof(I16) == 2);
    STATIC_REQUIRE(sizeof(U16) == 2);
    STATIC_REQUIRE(sizeof(I32) == 4);
    STATIC_REQUIRE(sizeof(U32) == 4);
    STATIC_REQUIRE(sizeof(I64) == 8);
    STATIC_REQUIRE(sizeof(U64) == 8);
    STATIC_REQUIRE(sizeof(F32) == 4);
    STATIC_REQUIRE(sizeof(F64) == 8);
    STATIC_REQUIRE(sizeof(Usize) == sizeof(void*));
    STATIC_REQUIRE(sizeof(Isize) == sizeof(void*));

    STATIC_REQUIRE(std::is_signed_v<I8>);
    STATIC_REQUIRE(std::is_unsigned_v<U8>);
    STATIC_REQUIRE(std::is_signed_v<I16>);
    STATIC_REQUIRE(std::is_unsigned_v<U16>);
    STATIC_REQUIRE(std::is_signed_v<I32>);
    STATIC_REQUIRE(std::is_unsigned_v<U32>);
    STATIC_REQUIRE(std::is_signed_v<I64>);
    STATIC_REQUIRE(std::is_unsigned_v<U64>);

    STATIC_REQUIRE(EPSILON > 0.0F);
}
