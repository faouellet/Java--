cmake_minimum_required(VERSION 2.8)
<<<<<<< HEAD
project(java--)
=======
>>>>>>> 76db62f952d353b5767f3116deabd520ddf9ed29

add_definitions(${CMAKE_CXX_FLAGS} -D__STDC_CONSTANT_MACROS)
add_definitions(${CMAKE_CXX_FLAGS} -D__STDC_FORMAT_MACROS)
add_definitions(${CMAKE_CXX_FLAGS} -D__STDC_LIMIT_MACROS)
add_definitions(${CMAKE_CXX_FLAGS} -Wall)
add_definitions(${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden)

<<<<<<< HEAD
if(NOT MSVC)
	set (CMAKE_CXX_FLAGS "-std=c++14 -fno-rtti" CACHE STRING "" FORCE)
    if($ENV{COVERAGE}==1)
        message("Adding flags for code coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
    endif()
endif()
=======
#if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-std=c++14 -fno-rtti")
    add_definitions(${CMAKE_CXX_FLAGS} -Wextra)
    #endif()

project(java--)

set(CMAKE_BUILD_TYPE Debug)
>>>>>>> 76db62f952d353b5767f3116deabd520ddf9ed29

add_subdirectory(src)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)
	add_custom_target(doc 
		COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
		COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
