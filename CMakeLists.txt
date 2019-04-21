cmake_minimum_required(VERSION 3.11)
project(faiz LANGUAGES CXX)
add_library(Faiz INTERFACE)
add_library(Rider::Faiz ALIAS Faiz)
target_include_directories(Faiz INTERFACE include/)

# if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
# 		add_compile_options(/Za)
# 		if(MSVC15)
# 			add_compile_options(/permissive-)
# 		endif()
# 	endif()


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

find_package(Mach7 QUIET)
  message("${BoldBlue}Looking for dependency Mach7----${ColourReset}" "${BoldYellow} Functional programming style pattern-matching library for C++${ColourReset}")

if(TARGET Mach7)
  message("${BoldGreen}Mach7 found\n${ColourReset}")
  # XXX: commented
  # include_directories(${Mach7_INCLUDE_DIRS})
else()
  message("${BoldGreen}====> Fetch Mach7... ${ColourReset}")
  include(FetchContent)
  # Download and configure Mach7 for the tests
  FetchContent_Declare(
    mach7
    GIT_REPOSITORY https://github.com/solodon4/Mach7/
    GIT_TAG master
    )
  FetchContent_GetProperties(mach7)
  if (NOT mach7_POPULATED)
    FetchContent_Populate(mach7)
    message("${Green}DONE!${ColourReset}")
    # add_subdirectory(${mach7_SOURCE_DIR}/code/test)
    # list(APPEND CMAKE_MODULE_PATH ${mach7_SOURCE_DIR}/contrib)
  endif()
endif()


find_package(Boost QUIET)
  message("${BoldBlue}Looking for dependency Boost---${ColourReset}" "${BoldYellow}Boost provides free peer-reviewed portable C++ source libraries.${ColourReset}")
if(Boost_FOUND)
  message("${BoldGreen}Boost found\n${ColourReset}")
else()
  message("${BoldGreen}====> Fetch Boost-predef and preprocessor... ${ColourReset}")
  include(FetchContent)
  FetchContent_Declare(
    predef
    GIT_REPOSITORY https://github.com/boostorg/predef/
    GIT_TAG master
    )
  FetchContent_Declare(
    preprocessor
    GIT_REPOSITORY https://github.com/boostorg/preprocessor/
    GIT_TAG master
    )

  FetchContent_GetProperties(predef)
  FetchContent_GetProperties(preprocessor)
  if (NOT predef_POPULATED AND NOT preprocessor_POPULATED)
    FetchContent_Populate(predef)
    FetchContent_Populate(preprocessor)
    add_subdirectory(${predef_SOURCE_DIR} ${predef_BINARY_DIR})
    add_subdirectory(${preprocessor_SOURCE_DIR} ${preprocessor_BINARY_DIR})
    message("${Green}DONE!${ColourReset}")
    target_link_libraries(Faiz
      INTERFACE
      boost_predef
      boost_preprocessor)
  endif()
endif()


# Include this module in the top CMakeLists.txt file of a project to enable testing with CTest and dashboard submissions to CDash:
include(CTest)
# The OLD behavior for this policy is to allow keyword and plain target_link_libraries signatures to be mixed. The NEW behavior for this policy is to not to allow mixing of the keyword and plain signatures.
cmake_policy(SET CMP0023 NEW)
add_subdirectory(test)
