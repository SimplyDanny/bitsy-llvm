#include <cctype>
#include <cstdlib>
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
        if (file_stream.eof()) {
            break;
        }
        if (isspace(current_character)) {
            advance();
        } else if (isdigit(current_character)) {
            tokens.push_back(Token(t_number, get_while_matching(isdigit)));
        } else if (is_operator(current_character)) {
            tokens.push_back(Token(t_operator, get_while_matching(is_operator)));
        } else if (current_character == '=') {
            tokens.push_back(Token(t_assignment, "="));
            advance();
        } else if (current_character == '(') {
            tokens.push_back(Token(t_left_parenthesis, "("));
            advance();
        } else if (current_character == ')') {
            tokens.push_back(Token(t_right_parenthesis, ")"));
            advance();
        } else if (is_identifier(current_character)) {
            auto token = get_while_matching(is_identifier);
            if (token == "BEGIN") {
                tokens.push_back(Token(t_begin, token));
            } else if (token == "END") {
                tokens.push_back(Token(t_end, token));
            } else if (token == "LOOP") {
                tokens.push_back(Token(t_loop, token));
            } else if (token == "BREAK") {
                tokens.push_back(Token(t_break, token));
            } else if (token == "IFN") {
                tokens.push_back(Token(t_ifn, token));
            } else if (token == "IFP") {
                tokens.push_back(Token(t_ifp, token));
            } else if (token == "IFZ") {
                tokens.push_back(Token(t_ifz, token));
            } else if (token == "ELSE") {
                tokens.push_back(Token(t_else, token));
            } else if (token == "PRINT") {
                tokens.push_back(Token(t_print, token));
            } else if (token == "READ") {
                tokens.push_back(Token(t_read, token));
            } else {
                tokens.push_back(Token(t_variable, token));
            }
        } else if (current_character == '{') {
            while (true) {
                advance();
                if (current_character == '}') {
                    advance();
                    break;
                }
            }
        }
    }
    return tokens;
}

void Lexer::advance() {
    current_character = file_stream.get();
}

std::string Lexer::get_while_matching(const TokenMatcher& matcher) {
    std::string value{};
    do {
        value += current_character;
        advance();
    } while (matcher(current_character));
    return value;
}
