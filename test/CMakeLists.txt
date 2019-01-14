# Copyright (C) 2018 Jonathan Müller <jonathanmueller.dev@gmail.com>
# This file is subject to the license terms in the LICENSE file
# found in the top-level directory of this distribution.

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/catch.hpp)
    file(DOWNLOAD
         https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp
         ${CMAKE_CURRENT_BINARY_DIR}/catch.hpp
         STATUS status
         LOG log)

    list(GET status 0 status_code)
    list(GET status 1 status_string)

    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "error downloading catch: ${status_string}"
                "${log}")
    endif()
endif()

set(tests
  cmp_nullptr.pass.cpp)

add_executable(faiz_test
                ${tests})
target_include_directories(faiz_test SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(faiz_test PUBLIC rider_faiz)
set_target_properties(faiz_test PROPERTIES CXX_STANDARD 20)

add_test(NAME test COMMAND faiz_test)

# add_subdirectory(unique_ptr)
enable_testing()
add_subdirectory(tight_pair)
