cmake_minimum_required(VERSION 3.24)
project(CALUMI.Animation.GTests)

# Setup GTest via FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(run_tests test_main.cpp)
target_link_libraries(run_tests PRIVATE GTest::gtest_main)


include(GoogleTest)
gtest_discover_tests(run_tests)