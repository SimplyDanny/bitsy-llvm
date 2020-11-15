#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "llvm/ADT/StringSwitch.h"

#include "lexer/Lexer.hpp"

const static Iterator iterator_end;

const static auto is_operator = [](auto c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
};

const static auto is_identifier = [](auto c) {
    return isalnum(c) || c == '_';
};

std::vector<Token> Lexer::get_tokens() {
    std::vector<Token> tokens;
    while (current_character != iterator_end) {
        if (isspace(*current_character) != 0) {
            ++current_character;
        } else if (isdigit(*current_character) != 0) {
            tokens.emplace_back(t_number, get_while_matching(isdigit));
        } else if (is_operator(*current_character)) {
            tokens.emplace_back(t_operator, get_while_matching(is_operator));
        } else if (*current_character == '=') {
            tokens.emplace_back(t_assignment, *current_character);
            ++current_character;
        } else if (*current_character == '(') {
            tokens.emplace_back(t_left_parenthesis, *current_character);
            ++current_character;
        } else if (*current_character == ')') {
            tokens.emplace_back(t_right_parenthesis, *current_character);
            ++current_character;
        } else if (is_identifier(*current_character)) {
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
        } else if (*current_character == '{') {
            current_character = std::next(std::find(current_character, iterator_end, '}'));
        } else {
            throw std::logic_error("Cannot handle the current character.");
        }
    }
    return tokens;
}

std::string Lexer::get_while_matching(const TokenMatcher& matcher) {
    std::string value;
    do {
        value += *current_character++;
    } while (current_character != iterator_end && matcher(*current_character));
    return value;
}
