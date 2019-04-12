find_package(Catch2 QUIET)
message("${BoldBlue}Looking for dependency Catch2----${ColourReset}" "${BoldYellow}A modern, C++-native, header-only, test framework for unit-tests, TDD and BDD - using C++11, C++14, C++17 and later (or C++03 on the Catch1.x branch)${ColourReset}")
if(TARGET Catch2::Catch2)
  message("${BoldGreen}Catch2 found\n${ColourReset}")
  include(Catch)
else()
  message("${Green}====> Fetch Catch2...${ColourReset}\n")

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
    message("${Green}DONE!${ColourReset}")
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
    include(Catch)

  message("${BoldBlue}git clone Catch2 successfully!${ColourReset}\n\n")
  endif()

endif()

include(ParseAndAddCatchTests)

set(tests
  cmp_nullptr.pass.cpp)

add_executable(faiz_test ${tests})
target_link_libraries(faiz_test
    PRIVATE
    Rider::Faiz
    Catch2::Catch2
)

set_target_properties(faiz_test PROPERTIES CXX_STANDARD 20)

ParseAndAddCatchTests(faiz_test)

enable_testing()
add_subdirectory(tight_pair/tests)
add_subdirectory(unique_ptr)
add_subdirectory(type_traits)
add_subdirectory(operators)
# add_subdirectory(containers/array)
# add_subdirectory(data/softfloat)
