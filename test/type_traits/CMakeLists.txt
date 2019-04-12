add_executable(
  type_traits-testsuite

  main.cpp
  add_const.libcxx.cpp
  add_cv.libcxx.cpp

  add_lvalue_reference.libcxx.cpp

  add_lvalue_reference.libstdcxx.cpp

  add_pointer.libcxx.cpp

  add_rvalue_reference.libstdcxx.cpp

  add_rvalue_refs.libcxx.cpp

  conditional.libcxx.cpp

  conjunction.libcxx.cpp

  decay.libcxx.cpp

  disjunction.libcxx.cpp

  extent.libcxx.cpp

  is_arithmetic.libcxx.cpp

  is_arithmetic.libstdcxx.cpp

  is_array.cpp

  is_array.libcxx.cpp

  is_assignable.libcxx.cpp

  is_assignable.libstdcxx.cpp

  is_base_of.libcxx.cpp

  is_class.libcxx.cpp

  is_class.libstdcxx.cpp

  is_convertible.libcxx.cpp

  is_convertible.libstdcxx.cpp

  is_copy_assignable.cpp

  is_copy_assignable.libstdcxx.cpp

  is_destructible.1.cpp

  is_destructible.cpp

  is_empty.libcxx.cpp

  is_floating_point.libcxx.cpp

  is_floating_point.libstdcxx.cpp

  is_function.libcxx.cpp

  is_integral.libcxx.cpp

  is_integral.libstdcxx.cpp

  is_member_function_pointer.libstdcxx.cpp

  is_member_pointer.libcxx.cpp

  is_member_pointer.libstdcxx.cpp

  is_move_assignable.libcxx.cpp

  is_nothrow_assignable.libcxx.cpp

  is_nothrow_copy_assignable.libcxx.cpp

  is_nothrow_destructible.libcxx.cpp

  is_nothrow_move_assignable.libcxx.cpp

  is_null_pointer.libcxx.cpp

  is_null_pointer.libstdcxx.cpp

  is_object.24808.cpp

  is_object.libcxx.cpp

  is_object.libstdcxx.cpp

  is_polymorphic.libcxx.cpp

  is_referenceable.libcxx.cpp

  is_rvalue_reference.libcxx.cpp

  is_same.libcxx.cpp

  is_signed.cpp

  is_trivially_copy_assignable.libcxx.cpp

  is_trivially_destructible.libcxx.cpp

  is_trivially_destructible.libstdcxx.cpp

  is_union.libcxx.cpp

  is_union.libstdcxx.cpp

  is_unsigned.libcxx.cpp

  is_unsigned.libstdcxx.cpp

  is_void.libcxx.cpp


  make_signed.1.cpp

  make_signed.alias.cpp

  make_signed.libcxx.cpp

  # make_signed.libstdcxx.typedef.neg.cpp

  make_unsigned.libcxx.cpp

  rank.libcxx.cpp

  remove_all_extents.libcxx.cpp

  remove_all_extents.libstdcxx.cpp

  remove_cv.libcxx.cpp

  remove_cvref.libcxx.cpp

  remove_cvref.libstdcxx.value.cpp

  remove_pointerlibcxx.cpp

  remove_pointer.libstdcxx.cpp

  test-utilities.hpp
  )


target_compile_features(type_traits-testsuite PUBLIC cxx_std_20)

# Somewhat speed up Catch2 compile times
target_compile_definitions(type_traits-testsuite PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_compile_definitions(type_traits-testsuite PRIVATE CATCH_CONFIG_DISABLE_MATCHERS)

# Use the gold linker if possible
if (UNIX AND NOT APPLE)
  set_property(TARGET type_traits-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=gold")
endif()

target_link_libraries(type_traits-testsuite
  PRIVATE
  Catch2::Catch2
  Rider::Faiz
  )

# Somewhat speed up Catch2 compile times
target_compile_definitions(type_traits-testsuite PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_compile_definitions(type_traits-testsuite PRIVATE CATCH_CONFIG_DISABLE_MATCHERS)

# Use the gold linker if possible
if (UNIX AND NOT APPLE)
  set_property(TARGET type_traits-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=gold")
endif()

# Optionally enable sanitizers
if (UNIX AND SANITIZE)
  target_compile_options(type_traits-testsuite PRIVATE -fsanitize=${SANITIZE})
  set_property(TARGET type_traits-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=${SANITIZE}")
endif()

catch_discover_tests(type_traits-testsuite)
