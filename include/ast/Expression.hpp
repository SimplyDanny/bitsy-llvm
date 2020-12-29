#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include <string>

#include "llvm/Support/Casting.h"

enum ExpressionKind { e_number, e_variable, e_binary_operation };

#define CLASS_OF_EXPRESSION(kind)                                                                                      \
    static bool classof(const Expression* statement) {                                                                 \
        return statement->get_kind() == kind;                                                                          \
    }

struct Expression {
    Expression(ExpressionKind kind)
        : kind(kind) {}

    [[nodiscard]] ExpressionKind get_kind() const {
        return kind;
    }

    virtual ~Expression() = default;

  private:
    const ExpressionKind kind;
};

struct NumberExpression : public Expression {
    int value;

    explicit NumberExpression(int value)
        : Expression(e_number)
        , value(value) {}

    CLASS_OF_EXPRESSION(e_number)
};

struct VariableExpression : public Expression {
    std::string name;

    explicit VariableExpression(std::string name)
        : Expression(e_variable)
        , name(std::move(name)) {}

    CLASS_OF_EXPRESSION(e_variable)
};

struct BinaryOperationExpression : public Expression {
    char operator_symbol;
    std::unique_ptr<Expression> left_expression;
    std::unique_ptr<Expression> right_expression;

    BinaryOperationExpression(char operator_symbol, std::unique_ptr<Expression> left_expression,
                              std::unique_ptr<Expression> right_expression)
        : Expression(e_binary_operation)
        , operator_symbol(operator_symbol)
        , left_expression(std::move(left_expression))
        , right_expression(std::move(right_expression)) {}

    CLASS_OF_EXPRESSION(e_binary_operation)
};

#endif
