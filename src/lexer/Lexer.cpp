#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

#include "llvm/ADT/StringSwitch.h"

#include "lexer/Lexer.hpp"

const static auto is_operator = [](const auto c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
};

const static auto is_identifier = [](const auto c) {
    return isalnum(c) || c == '_';
};

std::vector<Token> Lexer::get_tokens() {
    std::vector<Token> tokens{};
    advance();
    while (true) {
        if (isspace(current_character) != 0) {
            advance();
        } else if (isdigit(current_character) != 0) {
            tokens.emplace_back(t_number, get_while_matching(isdigit));
        } else if (is_operator(current_character)) {
            tokens.emplace_back(t_operator, get_while_matching(is_operator));
        } else if (current_character == '=') {
            tokens.emplace_back(t_assignment, "=");
            advance();
        } else if (current_character == '(') {
            tokens.emplace_back(t_left_parenthesis, "(");
            advance();
        } else if (current_character == ')') {
            tokens.emplace_back(t_right_parenthesis, ")");
            advance();
        } else if (is_identifier(current_character)) {
            auto token = get_while_matching(is_identifier);
            auto token_type = llvm::StringSwitch<TokenType>(token)
                                  .Case("BEGIN", t_begin)
                                  .Case("END", t_end)
                                  .Case("LOOP", t_loop)
                                  .Case("BREAK", t_break)
                                  .Case("IFN", t_ifn)
                                  .Case("IFP", t_ifp)
                                  .Case("IFZ", t_ifz)
                                  .Case("ELSE", t_else)
                                  .Case("PRINT", t_print)
                                  .Case("READ", t_read)
                                  .Default(t_variable);
            tokens.emplace_back(token_type, token);
        } else if (current_character == '{') {
            while (true) {
                advance();
                if (current_character == '}') {
                    advance();
                    break;
                }
            }
        } else if (current_character == '\0') {
            break;
        } else {
            throw std::logic_error("Cannot handle the current character.");
        }
    }
    return tokens;
}

void Lexer::advance() {
    current_character = file_stream.get();
    if (file_stream.eof()) {
        current_character = '\0';
    }
}

std::string Lexer::get_while_matching(const TokenMatcher& matcher) {
    std::string value{};
    do {
        value += current_character;
        advance();
    } while (matcher(current_character));
    return value;
}
