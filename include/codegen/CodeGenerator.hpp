#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

#include <map>
#include <stack>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "ast/ASTVisitor.hpp"

class CodeGenerator : public ASTVisitor<llvm::Value*> {
    llvm::Module& module;

    llvm::IRBuilder<> builder;

    bool had_break;

    llvm::Value* read_template;
    llvm::Value* print_template;

    llvm::Function* main_function;
    llvm::BasicBlock* main_block;

    std::map<std::string, llvm::Value*> known_variables;
    std::stack<llvm::BasicBlock*> loop_continuation_hierarchy;

  public:
    explicit CodeGenerator(llvm::Module& module);

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
