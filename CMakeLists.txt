cmake_minimum_required(VERSION 3.18)

project("bitsy-llvm")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

configure_file(include/helper/ClangPath.hpp.in include/helper/ClangPath.hpp)

include_directories(include ${PROJECT_BINARY_DIR}/include ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find the libraries that correspond to the LLVM components that we wish to use.
llvm_map_components_to_libnames(llvm_libs core)

add_executable(bitsyc
    src/bitsyc.cpp
    src/ast/ASTPrinter.cpp
    src/codegen/CodeGenerator.cpp
    src/helper/ConsolePrinter.cpp
    src/helper/PostProcessor.cpp
    src/lexer/Lexer.cpp
    src/lexer/Token.cpp
    src/parser/Parser.cpp
)

# Link against LLVM libraries.
target_link_libraries(bitsyc ${llvm_libs})
