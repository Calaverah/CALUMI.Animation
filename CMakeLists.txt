cmake_minimum_required(VERSION 3.26)
project(CALUMI.Animation VERSION 0.2.0.0 LANGUAGES CXX)


set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include GTest 
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#main build
set(PROJECT_SOURCES
    dllmain.cpp
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

add_subdirectory(include)
add_subdirectory(src)

target_compile_definitions(${PROJECT_NAME} PRIVATE CALUMIANIMATION_EXPORTS)

if(WIN32 AND MSVC)
    target_sources(${PROJECT_NAME} PRIVATE CALUMI.Animation.rc resource.h)
elseif(APPLE)

elseif(LINUX)
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RELEASE_BUILD)
endif()

target_include_directories(${PROJECT_NAME}
    
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/internal>
)

enable_testing()

add_executable(run_tests tests/test_main.cpp
    tests/math/quaternion.cpp
    tests/math/vector3.cpp
    tests/datacompression/sequence_compression.cpp 
    tests/mockdata/mock_sfbgs.cpp
    tests/univ/univ_entries.cpp 
    tests/sfbgs/datascan/sfbgs_animation_datascan.cpp
    tests/Common.h
    tests/sfbgs/datascan/sfbgs_rig_datascan.cpp
    tests/sfbgs/datascan/sfbgs_common.h
    tests/utilities/util_hash.cpp
    tests/math/transform.cpp
    tests/univ/univ_rig.cpp
    tests/json/json.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(run_tests PRIVATE DEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(run_tests PRIVATE RELEASE_BUILD)
endif()

add_custom_command(TARGET run_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${PROJECT_SOURCE_DIR}/tests/mockdata/assets"
    "$<TARGET_FILE_DIR:run_tests>/assets"
)

add_custom_command(TARGET run_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${PROJECT_SOURCE_DIR}/tests/paths"
    "$<TARGET_FILE_DIR:run_tests>/paths"
)

target_link_libraries(run_tests PRIVATE  ${PROJECT_NAME} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)
