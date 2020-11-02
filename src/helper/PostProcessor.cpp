#include <filesystem>
#include <random>
#include <string>

#include "helper/ClangPath.hpp"
#include "helper/PostProcessor.hpp"

static auto tmp_dir = std::filesystem::temp_directory_path() / "bitsyc";
static auto ll_file = tmp_dir / "tmp.ll";
static auto executable = tmp_dir / "tmp.out";

void ConsoleOutput::process(const llvm::Module* module) const {
    module->print(llvm::errs(), nullptr);
}

void ClangCompiler::process(const llvm::Module* module) const {
    std::filesystem::create_directory(tmp_dir);

    std::error_code error_code{};
    llvm::raw_fd_ostream file_stream{ll_file.string(), error_code};
    module->print(file_stream, nullptr);

    system(std::string(CLANG_PATH " ")
               .append(ll_file)
               .append(" -o ")
               .append(executable)
               .append(" -Wno-override-module") // For whatever reason the target triple returned by
                                                // 'llvm::sys::getDefaultTargetTriple()' is not the one Clang actually
                                                // expects on macOS. This option suppresses the warning.
               .c_str());
}

void Executer::process(const llvm::Module* module) const {
    system(executable.c_str());
}
