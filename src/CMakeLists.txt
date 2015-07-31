cmake_minimum_required(VERSION 2.8)

set(SOURCES 
    astprinter.cpp
    codegenerator.cpp
    driver.cpp
    main.cpp
    node.cpp
   )

find_package(BISON)
find_package(FLEX)

BISON_TARGET(parser
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp)

FLEX_TARGET(lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(frontend
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS})

find_package(LLVM REQUIRED COMPONENTS support core native)
if( NOT LLVM_FOUND )
    message(FATAL_ERROR "LLVM package can't be found. Set CMAKE_PREFIX_PATH variable to LLVM's installation prefix.")
endif()

include_directories( src ${LLVM_INCLUDE_DIRS} )
add_definitions(${LLVM_DEFINITIONS})
link_directories( ${LLVM_LIBRARY_DIRS} )

add_executable(java-- ${SOURCES})

llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES support core native)

target_link_libraries(java-- frontend ${REQ_LLVM_LIBRARIES})
