#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast/Statement.hpp"
#include "lexer/Token.hpp"

#include <memory>
#include <vector>

class Parser {
    std::vector<Token>::iterator token;

  public:
    explicit Parser(std::vector<Token> &tokens);
    std::unique_ptr<Program> parse();

  private:
    const Token *advance();
    std::unique_ptr<Expression> parse_expression();
    std::unique_ptr<Expression> parse_single_expression_component();
    std::unique_ptr<Expression> parse_parenthesis_expression();
    std::unique_ptr<Expression> parse_binary_expression(int precedence, std::unique_ptr<Expression> left_expression);
    std::unique_ptr<Block> parse_block(TokenType additional_stop_token = t_end);
    std::unique_ptr<Statement> parse_statement();
    std::unique_ptr<Statement> parse_if_statement(IfStatementType type);
};

#endif
