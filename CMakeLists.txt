# Copyright (C) 2018 Jonathan Müller <jonathanmueller.dev@gmail.com>
# This file is subject to the license terms in the LICENSE file
# found in the top-level directory of this distribution.

cmake_minimum_required(VERSION 3.1)
project(faiz)

set(header_files
        ${CMAKE_CURRENT_SOURCE_DIR}/include/rider/faiz/unique_ptr.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/rider/faiz/type_traits.hpp
    )
add_library(rider_faiz INTERFACE)
target_sources(rider_faiz INTERFACE ${header_files})
target_include_directories(rider_faiz INTERFACE include)
if(MSVC)
    target_compile_definitions(rider_faiz INTERFACE "-D_SCL_SECURE_NO_WARNINGS")
endif()

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    enable_testing()
    add_subdirectory(test)
endif()
