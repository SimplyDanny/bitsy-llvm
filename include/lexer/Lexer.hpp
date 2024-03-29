#ifndef LEXER_HPP
#define LEXER_HPP

#include "lexer/Token.hpp"

#include "llvm/ADT/StringSwitch.h"

#include <iterator>
#include <optional>
#include <string>
#include <type_traits>

template <class I>
concept CharIterator = std::same_as<typename std::iterator_traits<I>::value_type, char>;

template <CharIterator InputIterator>
class Lexer {

  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = Token;
    using difference_type = std::ptrdiff_t;
    using pointer = Token *;
    using reference = Token &;

  private:
    InputIterator current_character;
    InputIterator characters_end;

    std::optional<Token> current_token;

  public:
    Lexer(InputIterator begin, InputIterator end);
    Lexer() = default;

    Token operator*() const;
    Token &operator++();
    Token operator++(int);
    bool operator==(const Lexer &other) const;
    bool operator!=(const Lexer &other) const;

  private:
    std::optional<Token> next();
    template <class TokenMatcher>
        requires std::is_invocable_r_v<bool, TokenMatcher, char>
    std::string get_while_matching(const TokenMatcher &matcher);

    static bool is_operator(char c);
    static bool is_identifier(char c);
};

template <CharIterator InputIterator>
Lexer<InputIterator>::Lexer(InputIterator begin, InputIterator end)
  : current_character(begin)
  , characters_end(end) {
    if (current_character != characters_end) {
        ++(*this);
    }
}

template <CharIterator InputIterator>
Token Lexer<InputIterator>::operator*() const {
    return *current_token;
}

template <CharIterator InputIterator>
Token &Lexer<InputIterator>::operator++() {
    return *(current_token = next());
}

template <CharIterator InputIterator>
Token Lexer<InputIterator>::operator++(int) {
    auto tmp_token = std::move(current_token);
    ++(*this);
    return *tmp_token;
}

template <CharIterator InputIterator>
bool Lexer<InputIterator>::operator==(const Lexer &other) const {
    return current_token.has_value() == other.current_token.has_value();
}

template <CharIterator InputIterator>
bool Lexer<InputIterator>::operator!=(const Lexer &other) const {
    return !(*this == other);
}

template <CharIterator InputIterator>
std::optional<Token> Lexer<InputIterator>::next() {
    while (current_character != characters_end) {
        if (isspace(*current_character) != 0) {
            ++current_character;
        } else if (isdigit(*current_character) != 0) {
            return Token(TokenType::number_t, get_while_matching(isdigit));
        } else if (is_operator(*current_character)) {
            return Token(TokenType::operator_t, get_while_matching(is_operator));
        } else if (*current_character == '=') {
            return Token(TokenType::assignment_t, *current_character++);
        } else if (*current_character == '(') {
            return Token(TokenType::left_parenthesis_t, *current_character++);
        } else if (*current_character == ')') {
            return Token(TokenType::right_parenthesis_t, *current_character++);
        } else if (is_identifier(*current_character)) {
            auto token = get_while_matching(is_identifier);
            auto token_type = llvm::StringSwitch<TokenType>(token)
                                  .Case("BEGIN", TokenType::begin_t)
                                  .Case("END", TokenType::end_t)
                                  .Case("LOOP", TokenType::loop_t)
                                  .Case("BREAK", TokenType::break_t)
                                  .Case("IFN", TokenType::ifn_t)
                                  .Case("IFP", TokenType::ifp_t)
                                  .Case("IFZ", TokenType::ifz_t)
                                  .Case("ELSE", TokenType::else_t)
                                  .Case("PRINT", TokenType::print_t)
                                  .Case("READ", TokenType::read_t)
                                  .Default(TokenType::variable_t);
            return Token(token_type, token);
        } else if (*current_character == '{') {
            current_character = std::next(std::find(current_character, characters_end, '}'));
        } else {
            throw std::logic_error("Cannot handle the current character.");
        }
    }
    return {};
}

template <CharIterator InputIterator>
template <class TokenMatcher>
    requires std::is_invocable_r_v<bool, TokenMatcher, char>
std::string Lexer<InputIterator>::get_while_matching(const TokenMatcher &matcher) {
    std::string value;
    do {
        value += *current_character++;
    } while (current_character != characters_end && matcher(*current_character));
    return value;
}

template <CharIterator InputIterator>
bool Lexer<InputIterator>::is_operator(const char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

template <CharIterator InputIterator>
bool Lexer<InputIterator>::is_identifier(const char c) {
    return (isalnum(c) != 0) || c == '_';
}

#endif
