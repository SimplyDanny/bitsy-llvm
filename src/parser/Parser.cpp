#include <iostream>

#include "parser/Parser.hpp"

static auto log_expression_error(const char* string) {
    std::cerr << "Error parsing expression: " << string << std::endl;
    return nullptr;
}

static auto log_statement_error(const char* string) {
    std::cerr << "Error parsing statement: " << string << std::endl;
    return nullptr;
}

static IfStatementType get_if_type_from_token_type(const TokenType token_type) {
    switch (token_type) {
    case t_ifz:
        return zero;
    case t_ifp:
        return positive;
    case t_ifn:
        return negative;
    default:
        throw std::invalid_argument("No valid if-statement type for given token type.");
    }
}

static int get_operator_precedence(char operator_token) {
    switch (operator_token) {
    case '+':
    case '-':
        return 100;
    case '*':
    case '/':
    case '%':
        return 200;
    default:
        return -1;
    }
}

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {
    advance();
}

const Token* Parser::advance() {
    do {
        current_token = &tokens[++current_token_index];
    } while (current_token->type == t_comment || current_token->type == t_whitespace);
    return current_token;
}

std::unique_ptr<Program> Parser::parse() {
    if (current_token->type != t_begin) {
        return log_statement_error("Expecting token 'BEGIN'.");
    }
    auto block = parse_block();
    if (current_token->type != t_end) {
        return log_statement_error("Expecting token 'END'.");
    }
    return std::make_unique<Program>(std::move(block));
}

std::unique_ptr<Block> Parser::parse_block(const TokenType additional_stop_token) {
    std::vector<std::unique_ptr<Statement>> statements{};
    while (advance()->type != t_end && current_token->type != additional_stop_token) {
        statements.push_back(parse_statement());
    };
    return std::make_unique<Block>(std::move(statements));
}

std::unique_ptr<Expression> Parser::parse_expression() {
    if (auto left_expression = parse_single_expression_component()) {
        return parse_binary_expression(0, std::move(left_expression));
    }
    return nullptr;
}

std::unique_ptr<Expression> Parser::parse_single_expression_component() {
    switch (advance()->type) {
    case t_operator:
        if (current_token->value == "-" || current_token->value == "+") {
            return std::make_unique<NumberExpression>(std::stoi(current_token->value + advance()->value));
        }
        return log_expression_error("Unknown unary operator.");
    case t_number:
        return std::make_unique<NumberExpression>(std::stoi(current_token->value));
    case t_variable:
        return std::make_unique<VariableExpression>(current_token->value);
    case t_left_parenthesis:
        return parse_parenthesis_expression();
    default:
        return log_expression_error("Unknown token type during expression parsing.");
    }
}

std::unique_ptr<Expression> Parser::parse_parenthesis_expression() {
    if (current_token->type != t_left_parenthesis) {
        return log_expression_error("Expected opening parenthesis token.");
    }
    if (auto inner_expression = parse_expression()) {
        if (advance()->type != t_right_parenthesis) {
            return log_expression_error("Expected closing parenthesis token.");
        }
        return std::move(inner_expression);
    }
    return log_expression_error("Expression inside of parentheses cannot be parsed.");
}

std::unique_ptr<Expression> Parser::parse_binary_expression(int precedence,
                                                            std::unique_ptr<Expression> left_expression) {
    while (true) {
        auto next_token = tokens[current_token_index + 1];
        if (next_token.type != t_operator) {
            return left_expression;
        }
        char operator_token = next_token.value[0];
        int operator_precedence = get_operator_precedence(operator_token);
        if (operator_precedence < precedence) {
            return left_expression;
        }
        advance();
        auto right_expression = parse_single_expression_component();
        if (!right_expression) {
            return log_expression_error("Unable to parse right hand side expression.");
        }
        int next_operator_precedence = get_operator_precedence(tokens[current_token_index + 1].value[0]);
        if (operator_precedence < next_operator_precedence) {
            right_expression = parse_binary_expression(operator_precedence + 1, std::move(right_expression));
            if (!right_expression) {
                return nullptr;
            }
        }
        left_expression = std::make_unique<BinaryOperationExpression>(operator_token, std::move(left_expression),
                                                                      std::move(right_expression));
    }
}

std::unique_ptr<Statement> Parser::parse_statement() {
    switch (current_token->type) {
    case t_ifn:
    case t_ifp:
    case t_ifz: {
        auto type = get_if_type_from_token_type(current_token->type);
        auto expression = parse_expression();
        auto then_block = parse_block(t_else);
        auto else_block = current_token->type == t_else ? parse_block() : nullptr;
        return std::make_unique<IfStatement>(type, std::move(expression), std::move(then_block), std::move(else_block));
    }
    case t_loop: {
        auto block = parse_block();
        return std::make_unique<LoopStatement>(std::move(block));
    }
    case t_print: {
        auto expression = parse_expression();
        return std::make_unique<PrintStatement>(std::move(expression));
    }
    case t_read: {
        auto variable_expression = std::make_unique<VariableExpression>(advance()->value);
        return std::make_unique<ReadStatement>(std::move(variable_expression));
    }
    case t_break:
        return std::make_unique<BreakStatement>();
    case t_variable: {
        auto assignee = std::make_unique<VariableExpression>(current_token->value);
        if (advance()->type != t_assignment) {
            return log_statement_error("Expecting an assignment operator '='.");
        }
        auto assignment = parse_expression();
        return std::make_unique<AssignmentStatement>(std::move(assignee), std::move(assignment));
    }
    default:
        return log_statement_error("Unknown token type.");
    }
}
