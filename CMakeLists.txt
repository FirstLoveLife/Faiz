# Copyright (C) 2018 Jonathan Müller <jonathanmueller.dev@gmail.com>
# This file is subject to the license terms in the LICENSE file
# found in the top-level directory of this distribution.

cmake_minimum_required(VERSION 3.11)
project(faiz)

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

# find_package(Mach7 QUIET)
# if(TARGET Mach7::Mach7)
#   message("${BoldBlue}Mach7 has been installed previously\n\n${ColourReset}")
#   include_directories(${Mach7_INCLUDE_DIRS})

# else()

#   message("${Green}====> Downloading Mach7 ---- Functional programming style pattern-matching library for C++${ColourReset}")
#   include(FetchContent)
#   # Download and configure Mach7 for the tests
#   FetchContent_Declare(
#     mach7
#     GIT_REPOSITORY https://github.com/solodon4/Mach7/
#     GIT_TAG master
#     )

#   FetchContent_GetProperties(mach7)
#   if (NOT mach7_POPULATED)
#     FetchContent_Populate(mach7)

#     message(${mach7_SOURCE_DIR})
#     message(${mach7_BINARY_DIR})
#     add_subdirectory(${mach7_SOURCE_DIR}/code/test)
#     list(APPEND CMAKE_MODULE_PATH ${mach7_SOURCE_DIR}/contrib)
#     message("${BoldBlue}\ngit clone Mach7 successfully!\n\n${ColourReset}")
#   endif()

# endif()
find_package(FooBar)



  # include_directories(${root_SOURCE_DIR}/include)
  # Build tests if this is the main project
  enable_testing()
  add_subdirectory(test)
