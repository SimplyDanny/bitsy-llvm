#include <memory>

#include "codegen/CodeGenerator.hpp"
#include "codegen/ModuleBuilder.hpp"

std::unique_ptr<llvm::Module> ModuleBuilder::build() const {
    auto module = std::make_unique<llvm::Module>("Bitsy Program", context);

    CodeGenerator generator{*module};
    generator.visit(llvm::cast<Statement>(program));

    return module;
}
