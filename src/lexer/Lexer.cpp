#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

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
        if (isspace(current_character)) {
            advance();
        } else if (isdigit(current_character)) {
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
            if (token == "BEGIN") {
                tokens.emplace_back(t_begin, token);
            } else if (token == "END") {
                tokens.emplace_back(t_end, token);
            } else if (token == "LOOP") {
                tokens.emplace_back(t_loop, token);
            } else if (token == "BREAK") {
                tokens.emplace_back(t_break, token);
            } else if (token == "IFN") {
                tokens.emplace_back(t_ifn, token);
            } else if (token == "IFP") {
                tokens.emplace_back(t_ifp, token);
            } else if (token == "IFZ") {
                tokens.emplace_back(t_ifz, token);
            } else if (token == "ELSE") {
                tokens.emplace_back(t_else, token);
            } else if (token == "PRINT") {
                tokens.emplace_back(t_print, token);
            } else if (token == "READ") {
                tokens.emplace_back(t_read, token);
            } else {
                tokens.emplace_back(t_variable, token);
            }
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
