#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

#include <stdexcept>

#include "ast/Expression.hpp"
#include "ast/Statement.hpp"

template <class ExpressionReturnType>
class ASTVisitor {

  public:
    void visit(const Statement* statement);

  protected:
    virtual void visit(const Program* program) = 0;
    virtual void visit(const Block* block) = 0;
    virtual void visit(const IfStatement* if_statement) = 0;
    virtual void visit(const LoopStatement* loop_statement) = 0;
    virtual void visit(const PrintStatement* print_statement) = 0;
    virtual void visit(const ReadStatement* read_statement) = 0;
    virtual void visit(const AssignmentStatement* assignment_statement) = 0;
    virtual void visit(const BreakStatement* break_statement) = 0;

  public:
    ExpressionReturnType visit(const Expression* expression);

  protected:
    virtual ExpressionReturnType visit(const NumberExpression* number_expression) = 0;
    virtual ExpressionReturnType visit(const VariableExpression* variable_expression) = 0;
    virtual ExpressionReturnType visit(const BinaryOperationExpression* binary_operation_expression) = 0;
};

template <class ExpressionReturnType>
void ASTVisitor<ExpressionReturnType>::visit(const Statement* statement) {
    if (auto program = llvm::dyn_cast<Program>(statement)) {
        visit(program);
    } else if (auto block = llvm::dyn_cast<Block>(statement)) {
        visit(block);
    } else if (auto if_statement = llvm::dyn_cast<IfStatement>(statement)) {
        visit(if_statement);
    } else if (auto loop_statement = llvm::dyn_cast<LoopStatement>(statement)) {
        visit(loop_statement);
    } else if (auto print_statement = llvm::dyn_cast<PrintStatement>(statement)) {
        visit(print_statement);
    } else if (auto read_statement = llvm::dyn_cast<ReadStatement>(statement)) {
        visit(read_statement);
    } else if (auto assignment_statement = llvm::dyn_cast<AssignmentStatement>(statement)) {
        visit(assignment_statement);
    } else if (auto break_statement = llvm::dyn_cast<BreakStatement>(statement)) {
        visit(break_statement);
    } else {
        throw std::logic_error("Unknown 'Statement' type.");
    }
}

template <class ExpressionReturnType>
ExpressionReturnType ASTVisitor<ExpressionReturnType>::visit(const Expression* expression) {
    if (auto number_expression = llvm::dyn_cast<NumberExpression>(expression)) {
        return visit(number_expression);
    }
    if (auto variable_expression = llvm::dyn_cast<VariableExpression>(expression)) {
        return visit(variable_expression);
    }
    if (auto binary_operation_expression = llvm::dyn_cast<BinaryOperationExpression>(expression)) {
        return visit(binary_operation_expression);
    }
    throw std::logic_error("Unknown 'Expression' type.");
}

#endif
