cmake_minimum_required(VERSION 3.18)

project("bitsy-llvm")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_executable(bitsyc
    src/bitsyc.cpp
    src/ast/Expression.cpp
    src/ast/Statement.cpp
    src/lexer/Lexer.cpp
    src/lexer/Token.cpp
)
