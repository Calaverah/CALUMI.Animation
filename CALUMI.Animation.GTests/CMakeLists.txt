cmake_minimum_required(VERSION 3.24)
project(CALUMI.Animation.GTests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup GTest via FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../CALUMI.Animation/include)

enable_testing()

add_executable(run_tests test_main.cpp
  math/quaternion.cpp
  math/vector3.cpp
  datacompression/sequence_compression.cpp)

if(UNIX)
  set(LIB_PATH ${PROJECT_SOURCE_DIR}/../CALUMI.Animation/build/linux-release/libCALUMI.Animation.so)
elseif(APPLE)
  set(LIB_PATH )
elseif(WIN32)
  set(LIB_PATH )
endif()

target_link_libraries(run_tests PRIVATE ${LIB_PATH})

message(STATUS "Linking to: " ${LIB_PATH})

target_link_libraries(run_tests PRIVATE GTest::gtest_main)



include(GoogleTest)
gtest_discover_tests(run_tests)