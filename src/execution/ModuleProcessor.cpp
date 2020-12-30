#include <algorithm>
#include <filesystem>

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"

#include "execution/ModuleProcessor.hpp"
#include "helper/ClangPath.hpp"

const static auto tmp_dir = std::filesystem::temp_directory_path() / "bitsyc";
const static auto ll_file = tmp_dir / "tmp.ll";

void ModuleProcessor::print() const {
    module->print(llvm::outs(), nullptr);
}

void ModuleProcessor::compile() const {
    std::filesystem::create_directory(tmp_dir);

    std::error_code error_code;
    llvm::raw_fd_ostream file_stream{ll_file.string(), error_code};
    module->print(file_stream, nullptr);

    system(std::string(CLANG_PATH " ")
               .append(ll_file)
               .append(" -o ")
               .append(std::filesystem::current_path() / "a.out")
               .append(" -Wno-override-module") // For whatever reason the target triple returned by
                                                // 'llvm::sys::getDefaultTargetTriple()' is not the one Clang actually
                                                // expects on macOS. This option suppresses the warning.
               .c_str());
}

int ModuleProcessor::execute() {
    return system((std::filesystem::current_path() / "a.out").c_str());
}