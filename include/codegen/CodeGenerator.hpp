#ifndef CODEGENERATOR_HPP
#define CODEGENERATOR_HPP

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

class CodeGenerator {
    std::unique_ptr<Program> program;

    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value*> known_variables;

  public:
    CodeGenerator(std::unique_ptr<Program> program)
        : program(std::move(program)), builder(context),
          module(std::make_unique<llvm::Module>("Bitsy Program", context)){};
    void generate();

  private:
    void generate(const Statement* block);
    void generate(const Block* block);
    void generate(const IfStatement* if_statement);
    void generate(const LoopStatement* loop_statement);
    void generate(const PrintStatement* print_statement);
    void generate(const ReadStatement* read_statement);
    void generate(const AssignmentStatement* assignment_statement);
    void generate(const BreakStatement* break_statement);

    llvm::Value* generate(const Expression* expression);
    llvm::Value* generate(const NumberExpression* number_expression);
    llvm::Value* generate(const VariableExpression* variable_expression);
    llvm::Value* generate(const BinaryOperationExpression* binary_operation_expression);
};

#endif
