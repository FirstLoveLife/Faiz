# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")



find_package(Catch2 QUIET)
if(TARGET Catch2::Catch2)

  message("${BoldBlue}Catch2 has been installed previously\n\n${ColourReset}")
  include(Catch)
else()
  message("${Green}====> Catch2 hasn't not been installed on your os, git cloning into your build dir now${ColourReset}\n")

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

  message("${BoldBlue}git clone Catch2 successfully!${ColourReset}\n\n")
  endif()

endif()

set(tests
  cmp_nullptr.pass.cpp)

add_executable(faiz_test
  ${tests})
target_include_directories(faiz_test SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(faiz_test PROPERTIES CXX_STANDARD 20)

add_test(NAME test COMMAND faiz_test)


include_directories($(CMAKE_CURRENT_SOURCE_DIR)/include)

# add_subdirectory(unique_ptr)
enable_testing()
add_subdirectory(tight_pair)
add_subdirectory(unique_ptr)
add_subdirectory(type_traits)
# add_subdirectory(data/softfloat)
