#ifndef LEXER_HPP
#define LEXER_HPP

#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "llvm/ADT/StringSwitch.h"

#include "lexer/Token.hpp"

using TokenMatcher = std::function<bool(char)>;

template <class InputIterator>
class Lexer : public std::iterator<std::input_iterator_tag, Token> {

    static_assert(std::is_same<typename std::iterator_traits<InputIterator>::value_type, char>(),
                  "Expecting iterator over 'char' type.");

    InputIterator current_character;
    InputIterator characters_end;

    std::optional<Token> current_token;

  public:
    Lexer(InputIterator begin, InputIterator end);
    Lexer() = default;

    Token operator*() const;
    Token& operator++();
    Token operator++(int);
    bool operator==(const Lexer& other) const;
    bool operator!=(const Lexer& other) const;

  private:
    std::optional<Token> next();
    std::string get_while_matching(const TokenMatcher& matcher);

    static bool is_operator(char c);
    static bool is_identifier(char c);
};

template <class InputIterator>
Lexer<InputIterator>::Lexer(InputIterator begin, InputIterator end)
    : current_character(begin)
    , characters_end(end) {
    if (current_character != characters_end) {
        ++(*this);
    }
}

template <class InputIterator>
Token Lexer<InputIterator>::operator*() const {
    return *current_token;
}

template <class InputIterator>
Token& Lexer<InputIterator>::operator++() {
    return *(current_token = next());
}

template <class InputIterator>
Token Lexer<InputIterator>::operator++(int) {
    auto tmp_token = std::move(current_token);
    ++(*this);
    return *tmp_token;
}

template <class InputIterator>
bool Lexer<InputIterator>::operator==(const Lexer& other) const {
    return current_token.has_value() == other.current_token.has_value();
}

template <class InputIterator>
bool Lexer<InputIterator>::operator!=(const Lexer& other) const {
    return !(*this == other);
}

template <class InputIterator>
std::optional<Token> Lexer<InputIterator>::next() {
    while (current_character != characters_end) {
        if (isspace(*current_character) != 0) {
            ++current_character;
        } else if (isdigit(*current_character) != 0) {
            return Token(t_number, get_while_matching(isdigit));
        } else if (is_operator(*current_character)) {
            return Token(t_operator, get_while_matching(is_operator));
        } else if (*current_character == '=') {
            return Token(t_assignment, *current_character++);
        } else if (*current_character == '(') {
            return Token(t_left_parenthesis, *current_character++);
        } else if (*current_character == ')') {
            return Token(t_right_parenthesis, *current_character++);
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
            return Token(token_type, token);
        } else if (*current_character == '{') {
            current_character = std::next(std::find(current_character, characters_end, '}'));
        } else {
            throw std::logic_error("Cannot handle the current character.");
        }
    }
    return {};
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
