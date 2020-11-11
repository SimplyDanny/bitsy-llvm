#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>
#include <functional>
#include <string>
#include <vector>

#include "lexer/Token.hpp"

typedef std::function<int(char)> TokenMatcher;

class Lexer {
    std::ifstream file_stream;
    char current_character = '\0';

  public:
    explicit Lexer(std::ifstream&& file_stream) : file_stream(std::move(file_stream)) {}
    std::vector<Token> get_tokens();

  private:
    void advance();
    std::string get_while_matching(const TokenMatcher& matcher);
};

#endif
