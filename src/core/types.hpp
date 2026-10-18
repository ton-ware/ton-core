#pragma once

/**
 * @file types.hpp
 * @brief Foundational primitive types and fixed-width aliases
 *
 * @author Edwin Jabłoński
 * @date 2026-10-18
 * @version 1.0
 *
 * @copyright Copyright (c) 2026 Edwin Jabłoński
 *
 * @details
 * Standardizes primitive scalar types across platforms and hardware architectures.
 */

#include <cstddef>
#include <cstdint>

namespace ton::core {
using I8 = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;
using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;
using F32 = float;
using F64 = double;
using Isize = std::ptrdiff_t;
using Usize = std::size_t;
} // namespace ton::core
