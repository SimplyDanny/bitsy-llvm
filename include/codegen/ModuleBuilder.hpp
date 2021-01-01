#ifndef IRMODULEBUILDER_HPP
#define IRMODULEBUILDER_HPP

#include "ast/Statement.hpp"

#include "llvm/IR/Module.h"

#include <memory>

class ModuleBuilder {
    const Program *program;

    mutable llvm::LLVMContext context;

  public:
    ModuleBuilder(const Program *program)
      : program(program) {}

    [[nodiscard]] std::unique_ptr<llvm::Module> build() const;
};

#endif
