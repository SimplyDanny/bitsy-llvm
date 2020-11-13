#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include <string>

struct Expression {
    virtual ~Expression() = default;
};

struct NumberExpression : public Expression {
    int value;

    explicit NumberExpression(int value)
        : value(value) {}
};

struct VariableExpression : public Expression {
    std::string name;

    explicit VariableExpression(std::string name)
        : name(std::move(name)) {}
};

struct BinaryOperationExpression : public Expression {
    char operator_symbol;
    std::unique_ptr<Expression> left_expression;
    std::unique_ptr<Expression> right_expression;

    BinaryOperationExpression(char operator_symbol, std::unique_ptr<Expression> left_expression,
                              std::unique_ptr<Expression> right_expression)
        : operator_symbol(operator_symbol)
        , left_expression(std::move(left_expression))
        , right_expression(std::move(right_expression)) {}
};

#endif
