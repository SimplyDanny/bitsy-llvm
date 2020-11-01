#ifndef ASTPRINTER_HPP
#define ASTPRINTER_HPP

#include "ast/ASTVisitor.hpp"

class ASTPrinter : public ASTVisitor<void> {

  public:
    using ASTVisitor<void>::visit;

  private:
    void visit(const Program* program);
    void visit(const Block* block);
    void visit(const IfStatement* if_statement);
    void visit(const LoopStatement* loop_statement);
    void visit(const PrintStatement* print_statement);
    void visit(const ReadStatement* read_statement);
    void visit(const AssignmentStatement* assignment_statement);
    void visit(const BreakStatement* break_statement);

    void visit(const NumberExpression* number_expression);
    void visit(const VariableExpression* variable_expression);
    void visit(const BinaryOperationExpression* binary_operation_expression);
};

#endif
