cmake_minimum_required(VERSION 3.1.0)

#
# tight_pair is a header-only library. Therefore, no build whatsoever
# is needed to use the project. However, it includes a testsuite that
# can optionally be built with CMake. This is what this script does.
#
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

add_compile_options(-std=c++1z)

# Optionally enable sanitizers
if (UNIX AND SANITIZE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SANITIZE}")
endif()

# Include main header header
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Include Catch in the project and build the testsuite
# add_subdirectory(third_party/catch2)
add_subdirectory(tests)
