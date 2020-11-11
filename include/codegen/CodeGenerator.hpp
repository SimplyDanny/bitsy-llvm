#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "ast/ASTVisitor.hpp"
#include "helper/PostProcessor.hpp"

typedef std::function<void(const llvm::Module*)> ModuleProcessor;

class CodeGenerator : public ASTVisitor<llvm::Value*> {
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value*> known_variables;

    ModuleProcessor module_processor;

  public:
    explicit CodeGenerator(const ModuleProcessor& module_processor)
        : builder(context), module(std::make_unique<llvm::Module>("Bitsy Program", context)),
          module_processor(module_processor) {}

  public:
    using ASTVisitor<llvm::Value*>::visit;

  private:
    void visit(const Program* program);
    void visit(const Block* block);
    void visit(const IfStatement* if_statement);
    void visit(const LoopStatement* loop_statement);
    void visit(const PrintStatement* print_statement);
    void visit(const ReadStatement* read_statement);
    void visit(const AssignmentStatement* assignment_statement);
    void visit(const BreakStatement* break_statement);

    llvm::Value* visit(const NumberExpression* number_expression);
    llvm::Value* visit(const VariableExpression* variable_expression);
    llvm::Value* visit(const BinaryOperationExpression* binary_operation_expression);

    llvm::Value* allocate_variable(const std::string& name);
    llvm::Value* create_if_condition(const IfStatement* expression);
};

#endif
