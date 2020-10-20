#ifndef STATEMENT_HPP
#define STATEMENT_HPP

#include <memory>
#include <vector>

#include "ast/Expression.hpp"

enum IfStatementType { zero, positive, negative };

struct Statement {
    virtual ~Statement() = default;
    virtual void print() const = 0;
};

class Block : public Statement {
    std::vector<std::unique_ptr<Statement>> statements;

  public:
    Block(std::vector<std::unique_ptr<Statement>> statements) : statements(std::move(statements)) {}
    void print() const;
};

class Program : public Statement {
    std::unique_ptr<Block> block;

  public:
    Program(std::unique_ptr<Block> block) : block(std::move(block)){};
    void print() const;
};

class IfStatement : public Statement {
    IfStatementType type;
    std::unique_ptr<Expression> expression;
    std::unique_ptr<Block> then_block;
    std::unique_ptr<Block> else_block;

  public:
    IfStatement(const IfStatementType type, std::unique_ptr<Expression> expression, std::unique_ptr<Block> then_block,
                std::unique_ptr<Block> else_block = nullptr)
        : type(type), expression(std::move(expression)), then_block(std::move(then_block)),
          else_block(std::move(else_block)){};
    void print() const;
};

class LoopStatement : public Statement {
    std::unique_ptr<Block> block;

  public:
    LoopStatement(std::unique_ptr<Block> block) : block(std::move(block)){};
    void print() const;
};

class PrintStatement : public Statement {
    std::unique_ptr<Expression> expression;

  public:
    PrintStatement(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {}
    void print() const;
};

class ReadStatement : public Statement {
    std::unique_ptr<VariableExpression> variable_expression;

  public:
    ReadStatement(std::unique_ptr<VariableExpression> variable_expression)
        : variable_expression(std::move(variable_expression)) {}
    void print() const;
};

class AssignmentStatement : public Statement {
    std::unique_ptr<VariableExpression> variable;
    std::unique_ptr<Expression> expression;

  public:
    AssignmentStatement(std::unique_ptr<VariableExpression> variable, std::unique_ptr<Expression> expression)
        : variable(std::move(variable)), expression(std::move(expression)) {}
    void print() const;
};

class BreakStatement : public Statement {
  public:
    BreakStatement() {}
    void print() const;
};

#endif
