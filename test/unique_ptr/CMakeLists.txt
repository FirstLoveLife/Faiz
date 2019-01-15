cmake_minimum_required(VERSION 3.13)
# Define test files
#
#cmake_minimum_required(VERSION 3.13)
include(FetchContent)

# Download and configure Catch2 for the tests
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG master
)

FetchContent_GetProperties(catch2)
if (NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
    include(Catch)
endif()


set(
  assign

  assign/nullptr.cc
  assign/move.cc
  assign/move_array.cc
  assign/cv_qual.cc
  assign/48635.cc
  )

set(comparison

  comparision/42925.cc
  comparision/dr1401.cc
  )

set(cons
  # cons/77990.cc
  # cons/auto_ptr.cc
  cons/constexpr.cc
  cons/cv_qual.cc
  cons/default.cc
  cons/incomplete.cc
  cons/lwg2905.cc
  cons/noexcept_move_construct.cc
  cons/nullptr.cc
  cons/pointer_array.cc
  cons/pointer.cc
  cons/ptr_deleter.cc
)

set(creation
  creation/array.cc
  creation/single.cc
)

set(modifiers
  modifiers/43183.cc
  modifiers/48398.cc
  modifiers/cv_qual.cc
)

set(requirements
  requirements/explicit_instantiation/explicit_instantiation.cc
  requirements/incomplete.cc
  requirements/pointer_type_array.cc
  requirements/pointer_type.cc
)

set(specialized_algorithms
  specialized_algorithms/comparisons_array.cc
  specialized_algorithms/comparisons.cc
  specialized_algorithms/swap.cc
)



# Make one executable for the whole testsuite
add_executable(
  unique_ptr-testsuite

  main.cpp
  # 54351.cc https://wandbox.org/permlink/A3PJ2NwXVjhRBsCT clang cannot pass this test file
  dr2228.cc
  ${specialized_algorithms}
  ${requirements}
  ${modifiers}
  ${creation}
  ${cons}
  ${comparision}
  ${assign}
  )


target_compile_features(unique_ptr-testsuite PUBLIC cxx_std_20)
target_link_libraries(unique_ptr-testsuite
  PRIVATE
  Catch2::Catch2
  )

# Somewhat speed up Catch2 compile times
target_compile_definitions(unique_ptr-testsuite PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_compile_definitions(unique_ptr-testsuite PRIVATE CATCH_CONFIG_DISABLE_MATCHERS)



# Use the gold linker if possible
if (UNIX AND NOT APPLE)
  set_property(TARGET unique_ptr-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fuse-ld=gold")
endif()


include_directories(/home/firstlove/projects/faiz/include)

enable_testing()

add_test(NAME test COMMAND unique_ptr-testsuite)

# Optionally enable sanitizers
# if (UNIX AND SANITIZE)
  # target_compile_options(unique_ptr-testsuite PRIVATE -fsanitize=${SANITIZE})
  # set_property(TARGET unique_ptr-testsuite APPEND_STRING PROPERTY LINK_FLAGS " -fsanitize=${SANITIZE}")
# endif()

# string(RANDOM LENGTH 5 ALPHABET 0123456789 RNG_SEED)
# catch_discover_tests(unique_ptr-testsuite EXTRA_ARGS --rng-seed ${RNG_SEED})
