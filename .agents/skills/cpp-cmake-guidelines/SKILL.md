---
name: cpp-cmake-guidelines
description: >-
  Strict guidelines for CMake configuration, 3-tier build architecture,
  standalone Clang on Windows, cross-platform/embedded portability, and SIMD
  math implementation. Activate this skill when editing CMake files, presets,
  or reviewing C++ math/engine architecture.
---

# CMake and C++ Engine Guidelines

When modifying CMake configurations, `CMakePresets.json`, or reviewing C++ math and engine code across `ton-core`, `ton-engine`, and `ton-silicon`, adhere strictly to the following rules:

---

## 1. CMake Flags & Target Isolation (No Global Pollution)
- **NEVER** set `CMAKE_CXX_FLAGS`, `CMAKE_CXX_FLAGS_DEBUG`, or other global flag variables directly when they can pollute third-party dependencies (like `Catch2`).
- **ALWAYS** prefer modern `INTERFACE` targets for compiler and linker flags (e.g., `ton_compiler_flags`) configured with `target_compile_options` and `target_link_options`, linked `PRIVATE` or `INTERFACE` only to internal TON targets.
- When matching compiler IDs, always use `if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")`, **NOT** `STREQUAL` (since `AppleClang` or `Clang` variants can differ).
- Gate architecture-specific flags (such as `-msse4.1`) behind `CMAKE_SYSTEM_PROCESSOR` checks so builds remain portable across `x86_64`, `aarch64` (Android/ARM), and `Xtensa`/`RISC-V` (ESP32).
- In `ton-core`, always gate standalone-only targets (unit tests, benchmarks, Doxygen, `compile_commands.json` copying) with `${PROJECT_IS_TOP_LEVEL}` defaults so downstream `FetchContent` consumers do not inherit unnecessary build targets.

---

## 2. The 3-Tier Build Architecture & Sanitizers
- **Debug (`debug`)**: (`-O0 -g`) Used purely for step-debugging.
  - **NEVER enable sanitizers (ASan/UBSan) in Debug mode on Windows.** LLVM distributes prebuilt sanitizer runtimes compiled in Release mode (`_ITERATOR_DEBUG_LEVEL=0`), which causes fatal linker mismatches against MSVC Debug CRTs (`_ITERATOR_DEBUG_LEVEL=2`).
- **Development (`dev`)**: (`-O2 -g`) The daily-driver build.
  - Sanitizers (`-fsanitize=address`, `-fsanitize=undefined`), bounds checking (`-D_GLIBCXX_ASSERTIONS`), and runtime assertions MUST be placed here on supported host platforms.
- **Distribution (`dist`)**: (`-O3`) Ship build.
  - Maximum optimization; no sanitizers, no debug symbols, no debug assertions.

---

## 3. CMake Presets & Standalone Clang on Windows
- Use `CMakePresets.json` to define standard build tiers (`debug`, `dev`, `dist`) and workflows (`dev-pipeline`, `dist-pipeline`).
- For Clang + Ninja on Windows, specify `"architecture": { "value": "x64", "strategy": "external" }` in the host base preset. This allows CMake to automatically locate the Windows SDK and MSVC CRT without requiring a Visual Studio Developer Command Prompt or polluting global shell environments.
- **Do NOT duplicate variables** in `CMakePresets.json` that are already unconditionally set in `CMakeLists.txt` (such as `CMAKE_EXPORT_COMPILE_COMMANDS`).

---

## 4. ASan DLL Loading on Windows
- When AddressSanitizer is enabled on Windows with Clang, `clang_rt.asan_dynamic-x86_64.dll` must be dynamically located and copied to the test/executable directory post-build.
- **DO NOT** hardcode LLVM installation paths. Query the compiler dynamically:
  ```cmake
  execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-resource-dir
    OUTPUT_VARIABLE CLANG_RESOURCE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  ```

---

## 5. SIMD & Cross-Platform Math Rules
- **NEVER** use $16\text{-byte}$ SIMD load/store instructions (`_mm_loadu_ps`, `_mm_storeu_ps`, `vld1q_f32`) on $12\text{-byte}$ structs (like `Vec3`). Reading or writing $4\text{ bytes}$ past a $12\text{-byte}$ struct causes memory access violations and stack corruption.
- Apply SIMD only to naturally aligned $16\text{-byte}$ structures (`Vec4`, `Mat4`) or Struct-of-Arrays (SoA) batches.
- Standard `std::sqrt()` compiles to optimal hardware scalar instructions (`sqrtss` / `fsqrt`) and is faster than manual horizontal SIMD packing for single vectors.
- Always provide clean scalar fallbacks when neither `__x86_64__` nor `__aarch64__` is defined so `ton-core` compiles cleanly on ESP32 (`Xtensa` / `RISC-V`), and avoid accidental promotion of `F32` (`float`) to `F64` (`double`) on embedded targets where `double` is software-emulated.
