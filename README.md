# TON Core

```
+--------------------------------------------------------------------------------+
|                                                                                |
|                        ________  _______    _______                            |
|                       /        \/       \\//   /   \                           |
|                      /        _/        ///        /                           |
|                      /       //         /         /                            |
|                      \_____// \________/\__/_____/                             |
|                                                                                |
+--------------------------------------------------------------------------------+
```

TON Core is the foundational mathematics and systems library for the TON engine ecosystem. It provides portable scalar and SIMD math, fixed-point utilities, and core type definitions shared seamlessly by modern high-end platforms (TonEngine) and embedded microcontrollers (TonSilicon).

## Overview

**TON Core** serves as the single source of truth for shared foundations across the **TON** project:

- **`core/types.hpp` & `core/consts.hpp`**: Standardized fixed-width type aliases (`F32`, `F64`, `I8`, `UI8`, `I32`, `UI32`) and mathematical constants.
- **`core/math/`**: 2D/3D/4D vectors (`Vec2`, `Vec3`, `Vec4`), matrices, quaternions, and linear algebra utilities.
- **`core/math/simd*`**: Explicit, zero-overhead SIMD vector intrinsics with clean scalar fallbacks for architectures lacking vector extensions.

---

## SIMD Architecture Options

`TonCore` features an explicit SIMD configuration system controllable via the `TON_CORE_SIMD` CMake option:


| Option Value | Description | Injected Defines & Compiler Flags |
| :--- | :--- | :--- |
| **`AUTO`** *(Default)* | Auto-detects host CPU architecture | Selects `SSE41` on x86, `NEON` on ARM, or `NONE` on others |
| **`SSE41`** | Intel/AMD Streaming SIMD Extensions 4.1 | `-DTON_SIMD_SSE41 -DTON_SIMD_ENABLED=1`, `-msse4.1` |
| **`AVX2`** | Intel/AMD Advanced Vector Extensions 2 | `-DTON_SIMD_AVX2 -DTON_SIMD_ENABLED=1`, `-mavx2 -mfma` |
| **`NEON`** | ARM Advanced SIMD (Cortex / Apple Silicon) | `-DTON_SIMD_NEON -DTON_SIMD_ENABLED=1` |
| **`NONE`** | Pure scalar fallbacks (Embedded / ESP32 / Benchmarking) | `-DTON_SIMD_NONE -DTON_SIMD_ENABLED=0` |

---

## Requirements

### Standalone Development

- C++23 compiler (**Clang** preferred)
- **CMake** $\ge 3.28$
- **Ninja** build system
- **Catch2** `v3.5.2` (fetched automatically by CMake for tests and microbenchmarks)

### Consumed as a Subproject / Dependency

When consumed by downstream projects (`ton-engine` or `ton-silicon`) via `FetchContent` or `add_subdirectory`, unit tests and benchmarks are disabled automatically (`TON_CORE_BUILD_TESTS=OFF`), and the consumer's compiler flags govern the build.

---

## Building Standalone (Tests & Microbenchmarks)

`TonCore` includes CMake Presets for standalone testing and benchmarking:


| Tier | Configure Preset | Description |
| :--- | :--- | :--- |
| **Debug** | `debug` | `-O0 -g3` with full symbols for step-debugging, tests `ON` |
| **Development** | `dev` | `-O2 -g3` + Address/UB Sanitizers + hardening, tests `ON` |
| **Distribution** | `dist` | `-O3 -DNDEBUG` maximum optimization for microbenchmarking |

### Development Workflow

Configure:
```bash
cmake --preset dev
```

Build:
```bash
cmake --build --preset dev
```

Run tests:
```bash
ctest --preset dev
```

Or execute the complete workflow in one command:
```bash
cmake --workflow --preset dev-pipeline
```

---

## Project Structure

```text
ton-core/
├── CMakeLists.txt                  # Dual-mode CMake configuration & SIMD dispatch
├── CMakePresets.json               # Standalone test/benchmark presets
├── src/
│   └── core/                       # Core library root
│       ├── CMakeLists.txt
│       ├── consts.hpp              # Fundamental constants
│       ├── types.hpp               # Standardized type definitions
│       └── math/                   # Linear algebra foundation
│           ├── vec2.hpp / .cpp
│           ├── vec3.hpp / .cpp
│           ├── vec4.hpp / .cpp
│           ├── utils.hpp / .cpp
│           └── simd.hpp (Intel / ARM / Scalar)
└── test/                           # Catch2 unit tests and microbenchmarks
    ├── CMakeLists.txt
    └── core/
        └── sanity_tests.cpp
```

---

## License

This repository is protected by the [MIT](/LICENSE.md) license (except when stated otherwise).
