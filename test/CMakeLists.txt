fetchcontent_declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
  SYSTEM
)
fetchcontent_makeavailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

add_executable(TonCoreTests
  core/sanity_tests.cpp
  math/point2_tests.cpp
)

target_link_libraries(TonCoreTests
  PRIVATE
    Ton::Core
    Catch2::Catch2WithMain
)

# Catch2 macros use exceptions internally on host test runners; re-enable on test runner TU
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(TonCoreTests PRIVATE -fexceptions -w)
  set_target_properties(TonCoreTests PROPERTIES CXX_CLANG_TIDY "")
endif()

# Automatically locate and copy the ASan DLL on Windows with Clang
if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-resource-dir
    OUTPUT_VARIABLE CLANG_RESOURCE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  set(ASAN_DLL "${CLANG_RESOURCE_DIR}/lib/windows/clang_rt.asan_dynamic-x86_64.dll")
  if(EXISTS "${ASAN_DLL}")
    add_custom_command(TARGET TonCoreTests POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ASAN_DLL}"
        $<TARGET_FILE_DIR:TonCoreTests>
      COMMENT "Dynamically copying ASan DLL from compiler resources"
    )
  endif()
endif()

include(CTest)
include(Catch)

catch_discover_tests(TonCoreTests)
