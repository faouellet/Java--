cmake_minimum_required(VERSION 2.8)
project(java--)

add_definitions(${CMAKE_CXX_FLAGS} -D__STDC_CONSTANT_MACROS)
add_definitions(${CMAKE_CXX_FLAGS} -D__STDC_FORMAT_MACROS)
add_definitions(${CMAKE_CXX_FLAGS} -D__STDC_LIMIT_MACROS)
add_definitions(${CMAKE_CXX_FLAGS} -Wall)
add_definitions(${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden)

set(CMAKE_EXE_LINKER_FLAGS)

if(NOT MSVC)
	set (CMAKE_CXX_FLAGS "-std=c++14 -fno-rtti" CACHE STRING "" FORCE)
    if($ENV{COVERAGE}==1)
        message("Adding flags for code coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
    endif()
endif()

add_subdirectory(src)

# TODO: Add target for doxygen documentation generation

