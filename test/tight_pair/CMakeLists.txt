set(
    LIBCXX_TESTS

    libcxx/assign_const_pair_U_V.cpp
    libcxx/assign_pair.cpp
    libcxx/assign_rv_pair.cpp
    libcxx/assign_rv_pair_U_V.cpp
    libcxx/assign_tuple.cpp
    libcxx/comparison.cpp
    libcxx/const_first_const_second.cpp
    libcxx/const_pair_U_V.cpp
    libcxx/copy_ctor.cpp
    libcxx/default.cpp
    libcxx/default_ctor.cpp
    libcxx/default-sfinae.cpp
    libcxx/dtor.cpp
    libcxx/get_const.cpp
    libcxx/get_const_rv.cpp
    libcxx/get_non_const.cpp
    libcxx/get_rv.cpp
    libcxx/implicit_deduction_guides.cpp
    libcxx/move_ctor.cpp
    libcxx/non_member_swap.cpp
    libcxx/pairs_by_type.cpp
    libcxx/piecewise_construct.cpp
    libcxx/rv_pair_U_V.cpp
    libcxx/special_member_generation.cpp
    libcxx/swap.cpp
    libcxx/trivial_copy_move.cpp
    libcxx/tuple_element.cpp
    libcxx/tuple_size.cpp
    libcxx/types.cpp
    libcxx/U_V.cpp
    libcxx/used_to_be_make_pair.cpp
)

# Make one executable for the whole testsuite
add_executable(
    tight_pair-testsuite

    main.cpp
    cppreference.cpp
    dr-811.cpp
    empty_base_get.cpp
    piecewise_no_copy_move.cpp
    reference_wrapper.cpp
    tricky_comparisons.cpp
    ${LIBCXX_TESTS}
)

target_compile_features(tight_pair-testsuite PUBLIC cxx_std_20)

target_link_libraries(tight_pair-testsuite
    PRIVATE
        Catch2::Catch2
        Rider::Faiz
)

target_compile_definitions(tight_pair-testsuite PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_compile_definitions(tight_pair-testsuite PRIVATE CATCH_CONFIG_DISABLE_MATCHERS)

# Use the gold linker if possible
if (UNIX AND NOT APPLE)
    set_property(TARGET tight_pair-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=gold")
endif()

# Optionally enable sanitizers
if (UNIX AND SANITIZE)
	target_compile_options(tight_pair-testsuite PRIVATE -fsanitize=${SANITIZE})
	set_property(TARGET tight_pair-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=${SANITIZE}")
endif()

catch_discover_tests(tight_pair-testsuite)
