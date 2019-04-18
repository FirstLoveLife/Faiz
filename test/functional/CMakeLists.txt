# Make one executable for the whole testsuite
add_executable(
  compare_functor-testsuite

  main.cpp
  compare.cpp
  )


target_compile_features(compare_functor-testsuite PUBLIC cxx_std_20)

# Somewhat speed up Catch2 compile times
target_compile_definitions(compare_functor-testsuite PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_compile_definitions(compare_functor-testsuite PRIVATE CATCH_CONFIG_DISABLE_MATCHERS)

# Use the gold linker if possible
if (UNIX AND NOT APPLE)
  set_property(TARGET compare_functor-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=gold")
endif()

target_link_libraries(compare_functor-testsuite
    PRIVATE
        Catch2::Catch2
        Rider::Faiz
)

# Somewhat speed up Catch2 compile times
target_compile_definitions(compare_functor-testsuite PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_compile_definitions(compare_functor-testsuite PRIVATE CATCH_CONFIG_DISABLE_MATCHERS)

# Use the gold linker if possible
if (UNIX AND NOT APPLE)
    set_property(TARGET compare_functor-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=gold")
endif()

# Optionally enable sanitizers
if (UNIX AND SANITIZE)
	target_compile_options(compare_functor-testsuite PRIVATE -fsanitize=${SANITIZE})
	set_property(TARGET compare_functor-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=${SANITIZE}")
endif()

catch_discover_tests(compare_functor-testsuite)
