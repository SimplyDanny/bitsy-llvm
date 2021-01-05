#include "execution/ModuleProcessor.hpp"

#include "helper/ClangPath.hpp"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/Cloning.h"

#include <filesystem>
#include <iostream>
#include <memory>

const static auto tmp_dir = std::filesystem::temp_directory_path() / "bitsyc";
const static auto ll_file = tmp_dir / "tmp.ll";

void ModuleProcessor::print() const {
    module->print(llvm::outs(), nullptr);
}

bool ModuleProcessor::verify() const {
    return llvm::verifyModule(*module, &llvm::outs());
}

void ModuleProcessor::optimize() {
    llvm::legacy::FunctionPassManager manager{module.get()};
    manager.add(llvm::createInstructionCombiningPass());
    manager.add(llvm::createReassociatePass());
    manager.add(llvm::createGVNPass());
    manager.add(llvm::createCFGSimplificationPass());
    manager.add(llvm::createPromoteMemoryToRegisterPass());
    manager.doInitialization();
    manager.run(*module->getFunction("main"));
    manager.doFinalization();
}

int ModuleProcessor::compile() const {
    std::filesystem::create_directory(tmp_dir);

    std::error_code error_code;
    llvm::raw_fd_ostream file_stream{ll_file.string(), error_code};
    if (error_code.value() != 0) {
        std::cerr << "Error creating temporary file for IR output." << std::endl;
        return error_code.value();
    }
    module->print(file_stream, nullptr);

    auto out_file = std::filesystem::current_path() / output_name;
    std::vector<llvm::StringRef> arguments{
        CLANG_PATH,
        ll_file.c_str(),
        "-o",
        out_file.c_str(),
        "-Wno-override-module" // For whatever reason the target triple returned by
                               // 'llvm::sys::getDefaultTargetTriple()' is not the one Clang
                               // actually expects on macOS. This option
                               // suppresses the warning.
    };
    return llvm::sys::ExecuteAndWait(CLANG_PATH, arguments);
}

int ModuleProcessor::execute() const {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    auto engine_module = llvm::CloneModule(*module);
    auto main = engine_module->getFunction("main");
    auto engine = llvm::EngineBuilder(std::move(engine_module)).create();
    auto result = engine->runFunction(main, {});

    return result.IntVal.getSExtValue();
}
