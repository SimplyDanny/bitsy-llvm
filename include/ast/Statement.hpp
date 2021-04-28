#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include "ast/Expression.hpp"

#include <memory>
#include <vector>

enum class IfStatementType : char { zero = 'Z', positive = 'P', negative = 'N' };

#define CLASS_OF_STATEMENT(kind)                      \
    static bool classof(const Statement *statement) { \
        return statement->get_kind() == kind;         \
    }

struct Statement {
  protected:
    enum Kind { block_stm, program_stm, if_stm, loop_stm, print_stm, read_stm, assignment_stm, break_stm };

  public:
    Statement(Kind kind)
      : kind(kind) {}

    [[nodiscard]] Kind get_kind() const {
        return kind;
    }

    virtual ~Statement() = default;

  private:
    const Kind kind;
};

struct Block : public Statement {
    std::vector<std::unique_ptr<Statement>> statements;

    explicit Block(std::vector<std::unique_ptr<Statement>> statements)
      : Statement(block_stm)
      , statements(std::move(statements)) {}

    CLASS_OF_STATEMENT(block_stm)
};

struct Program : public Statement {
    std::unique_ptr<Block> block;

    explicit Program(std::unique_ptr<Block> block)
      : Statement(program_stm)
      , block(std::move(block)) {}

    CLASS_OF_STATEMENT(program_stm)
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
      : Statement(if_stm)
      , type(type)
      , expression(std::move(expression))
      , then_block(std::move(then_block))
      , else_block(std::move(else_block)) {}

    CLASS_OF_STATEMENT(if_stm)
};

struct LoopStatement : public Statement {
    std::unique_ptr<Block> block;

    explicit LoopStatement(std::unique_ptr<Block> block)
      : Statement(loop_stm)
      , block(std::move(block)) {}

    CLASS_OF_STATEMENT(loop_stm)
};

struct PrintStatement : public Statement {
    std::unique_ptr<Expression> expression;

    explicit PrintStatement(std::unique_ptr<Expression> expression)
      : Statement(print_stm)
      , expression(std::move(expression)) {}

    CLASS_OF_STATEMENT(print_stm)
};

struct ReadStatement : public Statement {
    std::unique_ptr<VariableExpression> variable_expression;

    explicit ReadStatement(std::unique_ptr<VariableExpression> variable_expression)
      : Statement(read_stm)
      , variable_expression(std::move(variable_expression)) {}

    CLASS_OF_STATEMENT(read_stm)
};

struct AssignmentStatement : public Statement {
    std::unique_ptr<VariableExpression> variable;
    std::unique_ptr<Expression> expression;

    AssignmentStatement(std::unique_ptr<VariableExpression> variable, std::unique_ptr<Expression> expression)
      : Statement(assignment_stm)
      , variable(std::move(variable))
      , expression(std::move(expression)) {}

    CLASS_OF_STATEMENT(assignment_stm)
};

struct BreakStatement : public Statement {
    BreakStatement()
      : Statement(break_stm){};

    CLASS_OF_STATEMENT(break_stm)
};

#endif
