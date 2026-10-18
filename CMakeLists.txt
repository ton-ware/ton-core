cmake_minimum_required(VERSION 3.28)
project("TON Core"
  VERSION 0.1.0
  LANGUAGES C CXX
)

# ------------------------------------------------------------------------------
# Build & Architecture Options
# ------------------------------------------------------------------------------
option(TON_CORE_BUILD_TESTS "Build TonCore unit tests and benchmarks" ${PROJECT_IS_TOP_LEVEL})

set(TON_CORE_SIMD "AUTO" CACHE STRING "SIMD backend architecture (AUTO, SSE41, AVX2, NEON, NONE)")
set_property(CACHE TON_CORE_SIMD PROPERTY STRINGS "AUTO" "SSE41" "AVX2" "NEON" "NONE")

# ------------------------------------------------------------------------------
# Language & Tooling Defaults
# ------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MESSAGE_CONTEXT_SHOW TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS AND PROJECT_IS_TOP_LEVEL)
  add_custom_target(ton_core_copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_BINARY_DIR}/compile_commands.json"
      "${CMAKE_SOURCE_DIR}/build/compile_commands.json"
    COMMENT "Copying compile_commands.json to build/"
  )
endif()

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# ------------------------------------------------------------------------------
# 3-Tier Build Types (Debug, Development, Distribution)
# ------------------------------------------------------------------------------
set(BUILD_TYPES Debug Development Distribution)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Development CACHE STRING "Build type" FORCE)
endif()

# ------------------------------------------------------------------------------
# Scoped Compiler & Hardening Flags (INTERFACE Target — No Global Pollution)
# ------------------------------------------------------------------------------
add_library(ton_core_compiler_flags INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ton_core_compiler_flags INTERFACE
    -Wall
    -Wextra
    -Werror
    -Wconversion
    -Wsign-conversion
    -Wpedantic
    -Wimplicit-fallthrough
  )

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(ton_core_compiler_flags INTERFACE
      -O0
      -g3
    )
  elseif(CMAKE_BUILD_TYPE STREQUAL "Development")
    target_compile_options(ton_core_compiler_flags INTERFACE
      -O2
      -g3
      -D_GLIBCXX_ASSERTIONS
      -ftrivial-auto-var-init=zero
      -fstack-protector-strong
    )
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      target_compile_options(ton_core_compiler_flags INTERFACE
        -Wtautological-undefined-compare
      )
    endif()
    # Control-flow protection and Sanitizers on host development builds
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64" AND NOT CMAKE_CROSSCOMPILING)
      target_compile_options(ton_core_compiler_flags INTERFACE
        -fcf-protection=full
        -fsanitize=address
        -fsanitize=undefined
      )
      target_link_options(ton_core_compiler_flags INTERFACE
        -fsanitize=address
        -fsanitize=undefined
      )
    endif()
  elseif(CMAKE_BUILD_TYPE STREQUAL "Distribution")
    target_compile_options(ton_core_compiler_flags INTERFACE
      -O3
      -DNDEBUG
    )
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(ton_core_compiler_flags INTERFACE /W4 /WX)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(ton_core_compiler_flags INTERFACE /Od /Zi)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Development")
    target_compile_options(ton_core_compiler_flags INTERFACE
      /O2 /GS /sdl /guard:cf /analyze /Zi
    )
    target_link_options(ton_core_compiler_flags INTERFACE /DYNAMICBASE /NXCOMPAT /DEBUG)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Distribution")
    target_compile_options(ton_core_compiler_flags INTERFACE
      /O2 /DNDEBUG /GS /sdl /guard:cf /analyze
    )
    target_link_options(ton_core_compiler_flags INTERFACE /DYNAMICBASE /NXCOMPAT)
  endif()
endif()

# ------------------------------------------------------------------------------
# SIMD Configuration & Target Flags
# ------------------------------------------------------------------------------
add_library(ton_core_simd_flags INTERFACE)

# Resolve AUTO setting
set(ACTIVE_SIMD "${TON_CORE_SIMD}")
if(ACTIVE_SIMD STREQUAL "AUTO")
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|i[3-6]86")
    set(ACTIVE_SIMD "SSE41")
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64|armv8")
    set(ACTIVE_SIMD "NEON")
  else()
    set(ACTIVE_SIMD "NONE")
  endif()
endif()

message(STATUS "TonCore SIMD configuration: ${ACTIVE_SIMD} (Selection: ${TON_CORE_SIMD})")

if(ACTIVE_SIMD STREQUAL "SSE41")
  target_compile_definitions(ton_core_simd_flags INTERFACE TON_SIMD_SSE41 TON_SIMD_ENABLED=1)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ton_core_simd_flags INTERFACE -msse4.1)
  endif()
elseif(ACTIVE_SIMD STREQUAL "AVX2")
  target_compile_definitions(ton_core_simd_flags INTERFACE TON_SIMD_AVX2 TON_SIMD_ENABLED=1)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ton_core_simd_flags INTERFACE -mavx2 -mfma)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(ton_core_simd_flags INTERFACE /arch:AVX2)
  endif()
elseif(ACTIVE_SIMD STREQUAL "NEON")
  target_compile_definitions(ton_core_simd_flags INTERFACE TON_SIMD_NEON TON_SIMD_ENABLED=1)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND CMAKE_SYSTEM_PROCESSOR MATCHES "armv7")
    target_compile_options(ton_core_simd_flags INTERFACE -mfpu=neon)
  endif()
elseif(ACTIVE_SIMD STREQUAL "NONE")
  target_compile_definitions(ton_core_simd_flags INTERFACE TON_SIMD_NONE TON_SIMD_ENABLED=0)
endif()

# ------------------------------------------------------------------------------
# clang-tidy Integration
# ------------------------------------------------------------------------------
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
else()
  message(WARNING "clang-tidy not found! Static analysis will be skipped.")
endif()

# ------------------------------------------------------------------------------
# Core Library
# ------------------------------------------------------------------------------
add_subdirectory(src)

# ------------------------------------------------------------------------------
# Standalone Unit Tests & Microbenchmarks
# ------------------------------------------------------------------------------
if(TON_CORE_BUILD_TESTS AND NOT CMAKE_CROSSCOMPILING)
  enable_testing()
  add_subdirectory(test)
endif()
