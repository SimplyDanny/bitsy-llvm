#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

#include "lexer/TokenType.hpp"

struct Token {
    const TokenType type;
    const std::string value;

    Token(TokenType type, std::string value) : type(type), value(value) {}

    void print() const;
};

#endif
