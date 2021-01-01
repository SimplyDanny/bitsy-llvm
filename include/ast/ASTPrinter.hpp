#ifndef ASTPRINTER_HPP
#define ASTPRINTER_HPP

#include "ast/ASTVisitor.hpp"

class ASTPrinter : public ASTVisitor<void> {

  public:
    using ASTVisitor<void>::visit;

  private:
    void visit(const Program *program) override;
    void visit(const Block *block) override;
    void visit(const IfStatement *if_statement) override;
    void visit(const LoopStatement *loop_statement) override;
    void visit(const PrintStatement *print_statement) override;
    void visit(const ReadStatement *read_statement) override;
    void visit(const AssignmentStatement *assignment_statement) override;
    void visit(const BreakStatement *break_statement) override;

    void visit(const NumberExpression *number_expression) override;
    void visit(const VariableExpression *variable_expression) override;
    void visit(const BinaryOperationExpression *binary_operation_expression) override;
};

#endif
