#ifndef MODULEEXECUTOR_HPP
#define MODULEEXECUTOR_HPP

#include "llvm/IR/Module.h"

#include <memory>

class ModuleProcessor {

    std::unique_ptr<llvm::Module> module;
    std::string output_name;

  public:
    ModuleProcessor(std::unique_ptr<llvm::Module> module, std::string output_name)
      : module(std::move(module))
      , output_name(std::move(output_name)) {}

    void print() const;
    void optimize();

    [[nodiscard]] bool show_cfg() const;
    [[nodiscard]] bool verify() const;
    [[nodiscard]] int compile() const;
    [[nodiscard]] int execute() const;
};

#endif
