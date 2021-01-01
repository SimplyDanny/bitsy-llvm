#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "lexer/TokenType.hpp"

#include <string>

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, std::string value)
      : type(type)
      , value(std::move(value)) {}
    Token(TokenType type, char value)
      : Token(type, std::string(1, value)) {}

    void print() const;
};

#endif
