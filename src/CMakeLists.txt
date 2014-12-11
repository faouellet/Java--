cmake_minimum_required(VERSION 2.8)

file(GLOB SOURCES *.cpp)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(parser
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp)

FLEX_TARGET(lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(frontend
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS})

add_executable(java-- ${SOURCES})

target_link_libraries(java-- frontend)
