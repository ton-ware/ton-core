# ------------------------------------------------------------------------------
# TonCore — Shared Engine Foundation (Types, Constants, Math, SIMD)
# ------------------------------------------------------------------------------
# Add your .cpp implementation files to TON_CORE_SOURCES as you migrate/create them:
set(TON_CORE_SOURCES
  # math/vec2.cpp
  # math/vec3.cpp
  # math/vec4.cpp
  # math/utils.cpp
)

if(TON_CORE_SOURCES)
  add_library(TonCore STATIC ${TON_CORE_SOURCES})
  target_include_directories(TonCore
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/..
      ${CMAKE_CURRENT_SOURCE_DIR}
  )
  target_link_libraries(TonCore
    PUBLIC
      ton_core_simd_flags
    PRIVATE
      ton_core_compiler_flags
  )
  if(CLANG_TIDY_EXE)
    set_target_properties(TonCore PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
  endif()
else()
  # Header-only / interface mode until the first .cpp file is added to TON_CORE_SOURCES
  add_library(TonCore INTERFACE)
  target_include_directories(TonCore
    INTERFACE
      ${CMAKE_CURRENT_SOURCE_DIR}/..
      ${CMAKE_CURRENT_SOURCE_DIR}
  )
  target_link_libraries(TonCore
    INTERFACE
      ton_core_compiler_flags
      ton_core_simd_flags
  )
endif()

add_library(Ton::Core ALIAS TonCore)
