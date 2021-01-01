#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "ast/Expression.hpp"

#include <memory>
#include <vector>

enum IfStatementType { zero, positive, negative };

enum StatementKind { s_block, s_program, s_if, s_loop, s_print, s_read, s_assignment, s_break };

#define CLASS_OF_STATEMENT(kind)                      \
    static bool classof(const Statement *statement) { \
        return statement->get_kind() == kind;         \
    }

struct Statement {
    Statement(StatementKind kind)
      : kind(kind) {}

    [[nodiscard]] StatementKind get_kind() const {
        return kind;
    }

    virtual ~Statement() = default;

  private:
    const StatementKind kind;
};

struct Block : public Statement {
    std::vector<std::unique_ptr<Statement>> statements;

    explicit Block(std::vector<std::unique_ptr<Statement>> statements)
      : Statement(s_block)
      , statements(std::move(statements)) {}

    CLASS_OF_STATEMENT(s_block)
};

struct Program : public Statement {
    std::unique_ptr<Block> block;

    explicit Program(std::unique_ptr<Block> block)
      : Statement(s_program)
      , block(std::move(block)) {}

    CLASS_OF_STATEMENT(s_program)
};

struct IfStatement : public Statement {
    IfStatementType type;
    std::unique_ptr<Expression> expression;
    std::unique_ptr<Block> then_block;
    std::unique_ptr<Block> else_block;

    IfStatement(const IfStatementType type,
                std::unique_ptr<Expression> expression,
                std::unique_ptr<Block> then_block,
                std::unique_ptr<Block> else_block = nullptr)
      : Statement(s_if)
      , type(type)
      , expression(std::move(expression))
      , then_block(std::move(then_block))
      , else_block(std::move(else_block)) {}

    CLASS_OF_STATEMENT(s_if)
};

struct LoopStatement : public Statement {
    std::unique_ptr<Block> block;

    explicit LoopStatement(std::unique_ptr<Block> block)
      : Statement(s_loop)
      , block(std::move(block)) {}

    CLASS_OF_STATEMENT(s_loop)
};

struct PrintStatement : public Statement {
    std::unique_ptr<Expression> expression;

    explicit PrintStatement(std::unique_ptr<Expression> expression)
      : Statement(s_print)
      , expression(std::move(expression)) {}

    CLASS_OF_STATEMENT(s_print)
};

struct ReadStatement : public Statement {
    std::unique_ptr<VariableExpression> variable_expression;

    explicit ReadStatement(std::unique_ptr<VariableExpression> variable_expression)
      : Statement(s_read)
      , variable_expression(std::move(variable_expression)) {}

    CLASS_OF_STATEMENT(s_read)
};

struct AssignmentStatement : public Statement {
    std::unique_ptr<VariableExpression> variable;
    std::unique_ptr<Expression> expression;

    AssignmentStatement(std::unique_ptr<VariableExpression> variable, std::unique_ptr<Expression> expression)
      : Statement(s_assignment)
      , variable(std::move(variable))
      , expression(std::move(expression)) {}

    CLASS_OF_STATEMENT(s_assignment)
};

struct BreakStatement : public Statement {
    BreakStatement()
      : Statement(s_break){};

    CLASS_OF_STATEMENT(s_break)
};

#endif
