#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include <string>

struct Expression {
    virtual ~Expression() = default;
    virtual void print() const = 0;
};

class NumberExpression : public Expression {
    int value;

  public:
    NumberExpression(int value) : value(value){};
    void print() const;
};

class VariableExpression : public Expression {
    std::string name;

  public:
    VariableExpression(const std::string& name) : name(name) {}
    void print() const;
};

class BinaryOperationExpression : public Expression {
    char operator_symbol;
    std::unique_ptr<Expression> left_expression;
    std::unique_ptr<Expression> right_expression;

  public:
    BinaryOperationExpression(char operator_symbol, std::unique_ptr<Expression> left_expression,
                              std::unique_ptr<Expression> right_expression)
        : operator_symbol(operator_symbol), left_expression(std::move(left_expression)),
          right_expression(std::move(right_expression)) {}
    void print() const;
};

#endif
