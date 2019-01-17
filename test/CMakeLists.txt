include(GNUInstallDirs)

find_package(Catch2)
if(TARGET Catch2::Catch2)
  message("catch2 has been installed previously")
  include(Catch)
else()
  message("no target catch2")
  find_package(Catch2 REQUIRED)
  if($(Catch2_FOUND))
    MESSAGE(STATUS "use local found catch2")
  else ($(Catch2_FOUND))
    MESSAGE("couldn't find catch2, will download")
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

  endif()

endif()



set(tests
  cmp_nullptr.pass.cpp)

add_executable(faiz_test
  ${tests})
target_include_directories(faiz_test SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(faiz_test PROPERTIES CXX_STANDARD 20)

add_test(NAME test COMMAND faiz_test)


include_directories(/home/firstlove/projects/faiz/include)

# add_subdirectory(unique_ptr)
enable_testing()
add_subdirectory(tight_pair)
add_subdirectory(unique_ptr)
