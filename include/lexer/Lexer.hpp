#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>
#include <functional>
#include <string>
#include <vector>

#include "lexer/Token.hpp"

typedef std::function<int(unsigned char)> TokenMatcher;

class Lexer {
    std::ifstream& file_stream;
    unsigned char current_character = -1;

  public:
    explicit Lexer(std::ifstream& file_stream) : file_stream(file_stream) {}
    std::vector<Token> get_tokens();

  private:
    void advance();
    std::string get_while_matching(const TokenMatcher& matcher);
};

#endif
