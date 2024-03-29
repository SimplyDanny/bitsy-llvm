#include "ast/ASTPrinter.hpp"

#include "helper/ConsolePrinter.hpp"

void ASTPrinter::visit(const Program *program) {
    cout << "BEGIN" << endl;
    cout << [=, this]() {
        visit(program->block.get());
    };
    cout << "END" << endl;
}

void ASTPrinter::visit(const Block *block) {
    for (const auto &statement : block->statements) {
        visit(statement.get());
    }
}

void ASTPrinter::visit(const IfStatement *if_statement) {
    cout << "IF" << if_statement->type << " ";
    ASTVisitor::visit(if_statement->expression.get());
    cout << endl;
    cout << [=, this]() {
        visit(if_statement->then_block.get());
    };
    if (if_statement->else_block) {
        cout << "ELSE" << endl;
        cout << [=, this]() {
            visit(if_statement->else_block.get());
        };
    }
    cout << "END" << endl;
}

void ASTPrinter::visit(const LoopStatement *loop_statement) {
    cout << "LOOP" << endl;
    cout << [=, this]() {
        visit(loop_statement->block.get());
    };
    cout << "END" << endl;
}

void ASTPrinter::visit(const PrintStatement *print_statement) {
    cout << "PRINT ";
    visit(print_statement->expression.get());
    cout << endl;
}

void ASTPrinter::visit(const ReadStatement *read_statement) {
    cout << "READ ";
    visit(read_statement->variable_expression.get());
    cout << endl;
}

void ASTPrinter::visit(const AssignmentStatement *assignment_statement) {
    visit(assignment_statement->variable.get());
    cout << " = ";
    visit(assignment_statement->expression.get());
    cout << endl;
}

void ASTPrinter::visit(const BreakStatement * /*break_statement*/) {
    cout << "BREAK" << endl;
}

void ASTPrinter::visit(const NumberExpression *number_expression) {
    cout << number_expression->value;
}

void ASTPrinter::visit(const VariableExpression *variable_expression) {
    cout << variable_expression->name;
}

void ASTPrinter::visit(const BinaryOperationExpression *binary_operation_expression) {
    cout << "(";
    visit(binary_operation_expression->left_expression.get());
    cout << " " << binary_operation_expression->operator_symbol << " ";
    visit(binary_operation_expression->right_expression.get());
    cout << ")";
}
