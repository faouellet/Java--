cmake_minimum_required(VERSION 2.8)

add_definitions(${CMAKE_CXX_FLAGS} -D__STDC_CONSTANT_MACROS)
add_definitions(${CMAKE_CXX_FLAGS} -D__STDC_FORMAT_MACROS)
add_definitions(${CMAKE_CXX_FLAGS} -D__STDC_LIMIT_MACROS)
add_definitions(${CMAKE_CXX_FLAGS} -Wall)
add_definitions(${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden)

#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-std=c++14 -fno-rtti")
    add_definitions(${CMAKE_CXX_FLAGS} -Wextra)
    #endif()

project(java--)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

