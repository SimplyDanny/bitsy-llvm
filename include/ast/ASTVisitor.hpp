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
    if (auto program = dynamic_cast<const Program*>(statement)) {
        visit(program);
    } else if (auto block = dynamic_cast<const Block*>(statement)) {
        visit(block);
    } else if (auto if_statement = dynamic_cast<const IfStatement*>(statement)) {
        visit(if_statement);
    } else if (auto loop_statement = dynamic_cast<const LoopStatement*>(statement)) {
        visit(loop_statement);
    } else if (auto print_statement = dynamic_cast<const PrintStatement*>(statement)) {
        visit(print_statement);
    } else if (auto read_statement = dynamic_cast<const ReadStatement*>(statement)) {
        visit(read_statement);
    } else if (auto assignment_statement = dynamic_cast<const AssignmentStatement*>(statement)) {
        visit(assignment_statement);
    } else if (auto break_statement = dynamic_cast<const BreakStatement*>(statement)) {
        visit(break_statement);
    } else {
        throw std::logic_error("Unknown 'Statement' type.");
    }
}

template <class ExpressionReturnType>
ExpressionReturnType ASTVisitor<ExpressionReturnType>::visit(const Expression* expression) {
    if (auto number_expression = dynamic_cast<const NumberExpression*>(expression)) {
        return visit(number_expression);
    }
    if (auto variable_expression = dynamic_cast<const VariableExpression*>(expression)) {
        return visit(variable_expression);
    }
    if (auto binary_operation_expression = dynamic_cast<const BinaryOperationExpression*>(expression)) {
        return visit(binary_operation_expression);
    }
    throw std::logic_error("Unknown 'Expression' type.");
}

#endif
