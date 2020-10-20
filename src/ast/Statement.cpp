#include <iostream>
#include <ostream>
#include <string>

#include "ast/ConsolePrinter.hpp"
#include "ast/Statement.hpp"

void Program::print() const {
    cout << "BEGIN";
    cout.add_endl();
    cout.indented([&]() {
        block->print();
    });
    cout << "END";
    cout.add_endl();
}

void Block::print() const {
    for (const auto& statement : statements) {
        statement->print();
    }
}

void IfStatement::print() const {
    cout << "IF<" << type << "> ";
    expression->print();
    cout.add_endl();
    cout.indented([&]() {
        then_block->print();
    });
    if (else_block) {
        cout << "ELSE";
        cout.add_endl();
        cout.indented([&]() {
            else_block->print();
        });
    }
    cout << "END";
    cout.add_endl();
}

void LoopStatement::print() const {
    cout << "LOOP";
    cout.add_endl();
    cout.indented([&]() {
        block->print();
    });
    cout << "END";
    cout.add_endl();
}

void PrintStatement::print() const {
    cout << "PRINT ";
    expression->print();
    cout.add_endl();
}

void ReadStatement::print() const {
    cout << "READ ";
    variable_expression->print();
    cout.add_endl();
}

void AssignmentStatement::print() const {
    variable->print();
    cout << " = ";
    expression->print();
    cout.add_endl();
}

void BreakStatement::print() const {
    cout << "BREAK";
    cout.add_endl();
}
