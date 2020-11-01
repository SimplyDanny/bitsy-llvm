#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "ast/ASTVisitor.hpp"
#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

class CodeGenerator : public ASTVisitor<llvm::Value*> {
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value*> known_variables;

  public:
    CodeGenerator() : builder(context), module(std::make_unique<llvm::Module>("Bitsy Program", context)){};

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
};

#endif
