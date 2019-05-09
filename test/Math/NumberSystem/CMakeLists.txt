add_executable(
  tools-testsuite
  Binary.cpp
  main.cpp
  )

target_compile_options(tools-testsuite
    PRIVATE
        -Wall -Wextra -Wcast-align -Winline -Wmissing-declarations -Wmissing-include-dirs
        -Wnon-virtual-dtor -Wodr -Wpedantic -Wredundant-decls -Wundef -Wunreachable-code
        $<$<CXX_COMPILER_ID:GNU>:-Wlogical-op -Wuseless-cast -Wzero-as-null-pointer-constant>
)

# Settings depending on debug/release mode
target_compile_options(tools-testsuite
    PRIVATE
        $<$<AND:$<CONFIG:DEBUG>,$<CXX_COMPILER_ID:Clang>>:-O0>
        $<$<AND:$<CONFIG:DEBUG>,$<CXX_COMPILER_ID:GNU>>:-Og>
)

target_compile_features(tools-testsuite PUBLIC cxx_std_20)

# Somewhat speed up Catch2 compile times
target_compile_definitions(tools-testsuite PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_compile_definitions(tools-testsuite PRIVATE CATCH_CONFIG_DISABLE_MATCHERS)

# Use lld or the gold linker if possible
if (UNIX AND NOT APPLE)
    set_property(TARGET tools-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=gold")
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set_property(TARGET tools-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=lld")
    else()
        set_property(TARGET tools-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=gold")
    endif()
endif()


target_link_libraries(tools-testsuite
  PRIVATE
  Catch2::Catch2
  Rider::Faiz
  )

# Somewhat speed up Catch2 compile times
target_compile_definitions(tools-testsuite PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_compile_definitions(tools-testsuite PRIVATE CATCH_CONFIG_DISABLE_MATCHERS)

# Optionally enable sanitizers
if (UNIX AND SANITIZE)
  target_compile_options(tools-testsuite PRIVATE -fsanitize=${SANITIZE})
  set_property(TARGET tools-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=${SANITIZE}")
endif()

catch_discover_tests(tools-testsuite)
