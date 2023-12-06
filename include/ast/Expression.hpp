#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <cstdint>
#include <memory>
#include <string>

#define CLASS_OF_EXPRESSION(kind)                      \
    static bool classof(const Expression *statement) { \
        return statement->get_kind() == kind;          \
    }

struct Expression {
  protected:
    enum Kind { number_expr, variable_expr, binary_operation_expr };

  public:
    Expression(Kind kind)
      : kind(kind) {}

    [[nodiscard]] Kind get_kind() const {
        return kind;
    }

    virtual ~Expression() = default;

  private:
    const Kind kind;
};

struct NumberExpression : public Expression {
    std::int32_t value;

    explicit NumberExpression(std::int32_t value)
      : Expression(number_expr)
      , value(value) {}

    CLASS_OF_EXPRESSION(number_expr)
};

struct VariableExpression : public Expression {
    std::string name;

    explicit VariableExpression(std::string name)
      : Expression(variable_expr)
      , name(std::move(name)) {}

    CLASS_OF_EXPRESSION(variable_expr)
};

struct BinaryOperationExpression : public Expression {
    char operator_symbol;
    std::unique_ptr<Expression> left_expression;
    std::unique_ptr<Expression> right_expression;

    BinaryOperationExpression(char operator_symbol,
                              std::unique_ptr<Expression> left_expression,
                              std::unique_ptr<Expression> right_expression)
      : Expression(binary_operation_expr)
      , operator_symbol(operator_symbol)
      , left_expression(std::move(left_expression))
      , right_expression(std::move(right_expression)) {}

    CLASS_OF_EXPRESSION(binary_operation_expr)
};

#endif
