#include <iostream>

#include "ast/ConsolePrinter.hpp"
#include "ast/Expression.hpp"

void NumberExpression::print() const {
    cout << value;
}

void VariableExpression::print() const {
    cout << name;
}

void BinaryOperationExpression::print() const {
    cout << "(";
    left_expression->print();
    cout << " " << operator_symbol << " ";
    right_expression->print();
    cout << ")";
}
