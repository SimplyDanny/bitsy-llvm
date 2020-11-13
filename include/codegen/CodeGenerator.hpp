#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "ast/ASTVisitor.hpp"
#include "helper/PostProcessor.hpp"

using ModuleProcessor = std::function<void(const llvm::Module*)>;

class CodeGenerator : public ASTVisitor<llvm::Value*> {
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value*> known_variables;

    ModuleProcessor module_processor;

  public:
    explicit CodeGenerator(ModuleProcessor module_processor)
        : builder(context), module(std::make_unique<llvm::Module>("Bitsy Program", context)),
          module_processor(std::move(module_processor)) {}

    using ASTVisitor<llvm::Value*>::visit;

  private:
    void visit(const Program* program) override;
    void visit(const Block* block) override;
    void visit(const IfStatement* if_statement) override;
    void visit(const LoopStatement* loop_statement) override;
    void visit(const PrintStatement* print_statement) override;
    void visit(const ReadStatement* read_statement) override;
    void visit(const AssignmentStatement* assignment_statement) override;
    void visit(const BreakStatement* break_statement) override;

    llvm::Value* visit(const NumberExpression* number_expression) override;
    llvm::Value* visit(const VariableExpression* variable_expression) override;
    llvm::Value* visit(const BinaryOperationExpression* binary_operation_expression) override;

    llvm::Value* allocate_variable(const std::string& name);
    llvm::Value* create_if_condition(const IfStatement* if_statement);
};

#endif
