#ifndef IRMODULEBUILDER_HPP
#define IRMODULEBUILDER_HPP

#include <memory>

#include "llvm/IR/Module.h"

#include "ast/Statement.hpp"

class ModuleBuilder {
    const Program* program;

    mutable llvm::LLVMContext context;

  public:
    ModuleBuilder(const Program* program)
        : program(program) {}

    [[nodiscard]] std::unique_ptr<llvm::Module> build() const;
};

#endif
