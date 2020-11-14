#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>
#include <functional>
#include <iterator>
#include <string>
#include <vector>

#include "lexer/Token.hpp"

using TokenMatcher = std::function<bool(char)>;
using Iterator = std::istreambuf_iterator<char>;

class Lexer {
    Iterator current_character;

  public:
    explicit Lexer(std::ifstream& file_stream)
        : current_character(file_stream) {}

    std::vector<Token> get_tokens();

  private:
    std::string get_while_matching(const TokenMatcher& matcher);
};

#endif
