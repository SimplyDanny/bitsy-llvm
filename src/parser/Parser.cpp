#include "parser/Parser.hpp"

#include "llvm/ADT/StringSwitch.h"

#include <iostream>
#include <stdexcept>

Parser::Parser(std::vector<Token> &tokens)
  : token(tokens.begin()) {
    if (token == tokens.end()) {
        throw std::logic_error("Got an invalid Bitsy program.");
    }
}

std::unique_ptr<Program> Parser::parse() {
    if (token->type != TokenType::begin_t) {
        throw std::logic_error("Expecting token 'BEGIN'.");
    }
    auto block = parse_block();
    if (token->type != TokenType::end_t) {
        throw std::logic_error("Expecting token 'END'.");
    }
    return std::make_unique<Program>(std::move(block));
}

std::unique_ptr<Block> Parser::parse_block(const TokenType additional_stop_token) {
    std::vector<std::unique_ptr<Statement>> statements;
    while ((++token)->type != TokenType::end_t && token->type != additional_stop_token) {
        statements.push_back(parse_statement());
    }
    return std::make_unique<Block>(std::move(statements));
}

std::unique_ptr<Expression> Parser::parse_expression() {
    if (auto left_expression = parse_single_expression_component()) {
        return parse_binary_expression(0, std::move(left_expression));
    }
    return nullptr;
}

std::unique_ptr<Expression> Parser::parse_single_expression_component() {
    switch ((++token)->type) {
        using enum TokenType;
        case operator_t: {
            auto op = token++->value;
            if (op == "-" || op == "+") {
                return std::make_unique<NumberExpression>(std::stol(op + token->value));
            }
            throw std::logic_error("Unknown unary operator '" + op + "'.");
        }
        case number_t:
            return std::make_unique<NumberExpression>(std::stol(token->value));
        case variable_t:
            return std::make_unique<VariableExpression>(token->value);
        case left_parenthesis_t:
            return parse_parenthesis_expression();
        default:
            throw std::logic_error("Unknown token type during expression parsing.");
    }
}

std::unique_ptr<Expression> Parser::parse_parenthesis_expression() {
    if (token->type != TokenType::left_parenthesis_t) {
        throw std::logic_error("Expected opening parenthesis token.");
    }
    if (auto inner_expression = parse_expression()) {
        if ((++token)->type != TokenType::right_parenthesis_t) {
            throw std::logic_error("Expected closing parenthesis token.");
        }
        return inner_expression;
    }
    throw std::logic_error("Expression inside of parentheses cannot be parsed.");
}

static int get_operator_precedence(const std::string &operator_token) {
    // clang-format off
    return llvm::StringSwitch<int>(operator_token)
               .Cases("+", "-", 100)
               .Cases("*", "/", "%", 200)
               .Default(-1);
    // clang-format on
}

std::unique_ptr<Expression> Parser::parse_binary_expression(int precedence,
                                                            std::unique_ptr<Expression> left_expression) {
    while (true) {
        if ((token + 1)->type != TokenType::operator_t) {
            return left_expression;
        }
        auto operator_token = (token + 1)->value;
        int operator_precedence = get_operator_precedence(operator_token);
        if (operator_precedence < precedence) {
            return left_expression;
        }
        ++token;
        auto right_expression = parse_single_expression_component();
        if (!right_expression) {
            throw std::logic_error("Unable to parse right hand side expression.");
        }
        int next_operator_precedence = get_operator_precedence((token + 1)->value);
        if (operator_precedence < next_operator_precedence) {
            right_expression = parse_binary_expression(operator_precedence + 1, std::move(right_expression));
            if (!right_expression) {
                return nullptr;
            }
        }
        left_expression = std::make_unique<BinaryOperationExpression>(operator_token[0],
                                                                      std::move(left_expression),
                                                                      std::move(right_expression));
    }
}

std::unique_ptr<Statement> Parser::parse_statement() {
    switch (token->type) {
        using enum TokenType;
        case ifn_t:
            return parse_if_statement(IfStatementType::negative);
        case ifp_t:
            return parse_if_statement(IfStatementType::positive);
        case ifz_t:
            return parse_if_statement(IfStatementType::zero);
        case loop_t:
            return std::make_unique<LoopStatement>(parse_block());
        case print_t:
            return std::make_unique<PrintStatement>(parse_expression());
        case read_t: {
            if ((++token)->type != variable_t) {
                throw std::logic_error("Expecting a variable as the argument of a 'READ' statement.");
            }
            auto variable_expression = std::make_unique<VariableExpression>(token->value);
            return std::make_unique<ReadStatement>(std::move(variable_expression));
        }
        case break_t:
            return std::make_unique<BreakStatement>();
        case variable_t: {
            auto assignee = std::make_unique<VariableExpression>(token->value);
            if ((++token)->type != assignment_t) {
                throw std::logic_error("Expecting an assignment operator '='.");
            }
            auto assignment = parse_expression();
            return std::make_unique<AssignmentStatement>(std::move(assignee), std::move(assignment));
        }
        default:
            throw std::logic_error("Unknown token type.");
    }
}

std::unique_ptr<Statement> Parser::parse_if_statement(const IfStatementType type) {
    auto expression = parse_expression();
    auto then_block = parse_block(TokenType::else_t);
    auto else_block = token->type == TokenType::else_t ? parse_block() : nullptr;
    return std::make_unique<IfStatement>(type, std::move(expression), std::move(then_block), std::move(else_block));
}
