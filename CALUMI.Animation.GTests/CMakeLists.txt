cmake_minimum_required(VERSION 3.26)
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
  datacompression/sequence_compression.cpp 
  mockdata/mock_sfbgs.cpp
  univ/univ_entries.cpp 
  sfbgs/datascan/sfbgs_animation_datascan.cpp
  Common.h
  sfbgs/datascan/sfbgs_rig_datascan.cpp
  sfbgs/datascan/sfbgs_common.h
        utilities/util_hash.cpp
        math/transform.cpp
        #univ/univ_rig.cpp
)

target_compile_definitions(run_tests PRIVATE DEBUG_BUILD)

if(LINUX)

  set(LIB_PATH ${PROJECT_SOURCE_DIR}/../CALUMI.Animation/build/linux-debug/libCALUMI.Animation.so)

elseif(APPLE)

  set(LIB_PATH ${PROJECT_SOURCE_DIR}/../CALUMI.Animation/build/mac-debug/libCALUMI.Animation.dylib)

elseif(WIN32)

    #lib path
  set(LIB_PATH ${PROJECT_SOURCE_DIR}/../CALUMI.Animation/build/win-debug/CALUMI.Animation.lib)
    #copy dll
  add_custom_command(TARGET run_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/../CALUMI.Animation/build/win-debug/CALUMI.Animation.dll"
        $<TARGET_FILE_DIR:run_tests>
      )

endif()

add_custom_command(TARGET run_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${PROJECT_SOURCE_DIR}/mockdata/assets"
    "$<TARGET_FILE_DIR:run_tests>/assets"
)

add_custom_command(TARGET run_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${PROJECT_SOURCE_DIR}/paths"
    "$<TARGET_FILE_DIR:run_tests>/paths"
)

target_link_libraries(run_tests PRIVATE ${LIB_PATH})

message(STATUS "Linking to: " ${LIB_PATH})

target_link_libraries(run_tests PRIVATE GTest::gtest_main)



include(GoogleTest)
gtest_discover_tests(run_tests)
