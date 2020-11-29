#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>
#include <functional>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

#include "llvm/ADT/StringSwitch.h"

#include "lexer/Token.hpp"

using TokenMatcher = std::function<bool(char)>;

template <class InputIterator>
class Lexer {

    static_assert(std::is_same<typename std::iterator_traits<InputIterator>::value_type, char>::value,
                  "Expecting iterator over 'char' type.");

    InputIterator current_character;
    InputIterator characters_end;

  public:
    explicit Lexer(InputIterator begin, InputIterator end)
        : current_character(begin)
        , characters_end(end) {}

    std::vector<Token> get_tokens();

  private:
    std::string get_while_matching(const TokenMatcher& matcher);

    static bool is_operator(char c);
    static bool is_identifier(char c);
};

template <class InputIterator>
std::vector<Token> Lexer<InputIterator>::get_tokens() {
    std::vector<Token> tokens;
    while (current_character != characters_end) {
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
            current_character = std::next(std::find(current_character, characters_end, '}'));
        } else {
            throw std::logic_error("Cannot handle the current character.");
        }
    }
    return tokens;
}

template <class InputIterator>
std::string Lexer<InputIterator>::get_while_matching(const TokenMatcher& matcher) {
    std::string value;
    do {
        value += *current_character++;
    } while (current_character != characters_end && matcher(*current_character));
    return value;
}

template <class InputIterator>
bool Lexer<InputIterator>::is_operator(const char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

template <class InputIterator>
bool Lexer<InputIterator>::is_identifier(const char c) {
    return (isalnum(c) != 0) || c == '_';
}

#endif
