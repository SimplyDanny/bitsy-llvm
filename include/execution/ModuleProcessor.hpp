#ifndef MODULEEXECUTOR_HPP
#define MODULEEXECUTOR_HPP

#include <memory>

#include "llvm/IR/Module.h"

class ModuleProcessor {

    std::unique_ptr<llvm::Module> module;

  public:
    ModuleProcessor(std::unique_ptr<llvm::Module> module)
        : module(std::move(module)) {}

    void print() const;
    void compile() const;
    int execute();
};

#endif