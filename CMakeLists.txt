cmake_minimum_required(VERSION 3.17)

project("bitsy-llvm")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS false)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in '${LLVM_DIR}'")

configure_file(include/helper/ClangPath.hpp.in include/helper/ClangPath.hpp)

include_directories(include ${PROJECT_BINARY_DIR}/include ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find the libraries that correspond to the LLVM components that we wish to use.
llvm_map_components_to_libnames(
    BITSYC_LLVM_LIBRARIES
    Core
    MCJIT
    nativecodegen
    Passes
)

add_executable(
    bitsyc
    src/bitsyc.cpp
    src/ast/ASTPrinter.cpp
    src/codegen/CodeGenerator.cpp
    src/codegen/ModuleBuilder.cpp
    src/execution/ModuleProcessor.cpp
    src/helper/ConsolePrinter.cpp
    src/parser/Parser.cpp
)

set_target_properties(bitsyc PROPERTIES VISIBILITY_INLINES_HIDDEN true)

if(NOT LLVM_ENABLE_RTTI)
    message(STATUS "Building without RTTI")
    target_compile_options(bitsyc PRIVATE -fno-rtti)
endif()

# Link against LLVM libraries.
target_link_libraries(bitsyc ${BITSYC_LLVM_LIBRARIES})
