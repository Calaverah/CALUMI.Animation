cmake_minimum_required(VERSION 3.24)
project(CALUMI.Animation VERSION 0.2.0.0 LANGUAGES CXX)


set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    dllmain.cpp
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

add_subdirectory(include)
add_subdirectory(src)

target_compile_definitions(${PROJECT_NAME} PRIVATE CALUMIANIMATION_EXPORTS)

if(WIN32 AND MSVC)
    target_sources(${PROJECT_NAME} PRIVATE CALUMI.Animation.rc resource.h)
elseif(APPLE)

elseif(LINUX)
#    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
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


set_target_properties(${PROJECT_NAME} PROPERTIES 
CXX_VISIBILITY_PRESET hidden
VISIBILITY_INLINES_HIDDEN ON
)
