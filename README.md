# BitsyLLVM

![CMake Build](https://github.com/SimplyDanny/bitsy-llvm/workflows/CMake%20Build/badge.svg)

BitsyLLVM (also "bitsyc") is a compiler for the
[Bitsy](https://github.com/apbendi/bitsyspec) language implemented in C++ using
the [LLVM](https://llvm.org) framework to handle the code generation step.

## Building

After cloning this repository, create a `build` folder at the project root,
enter it

    cd bitsy-llvm
    mkdir build
    cd build

and run CMake like:

    cmake                                       \
        -DCMAKE_C_COMPILER=clang                \
        -DCMAKE_CXX_COMPILER=clang++            \
        -DLLVM_DIR=/path/to/llvm/installation   \
        ..

Make sure that there is an installation of LLVM on your system. It is necessary
that the Clang compiler is used to build bitsyc and that CMake knows where to
find the LLVM configuration (`LLVM_DIR`). Make sure that the values of
`CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER` can be found in the `PATH` or use
absolute paths.

Finally, build bitsyc with the build tool (Make, Ninja, ...) CMake has chosen or
which you specified in the `cmake` call above with the `-G` argument. Depending
on the chosen build tool, the bitsyc executable is located somewhere in `build`,
typically (for Make, Ninja, ...) it is `build/bitsyc`.

## Usage

The compiler bitsyc requires one argument, namely the path to a `.bitsy` file
containing a Bitsy program. It will parse it, generate LLVM IR for it, pass this
intermediate code to the Clang compiler and execute the built binary. All files
generated will be put into a temporary directory on your system.

You may pass the path to bitsyc to the `runspec` script in the
[Bitsy](https://github.com/apbendi/bitsyspec) repository to run all its
[reference tests](https://github.com/apbendi/bitsyspec#usage) against it.
