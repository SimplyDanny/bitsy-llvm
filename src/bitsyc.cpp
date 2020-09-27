#include <fstream>
#include <iostream>

#include "lexer/Lexer.hpp"
#include "lexer/TokenType.hpp"

int main() {
    std::ifstream file_stream{"example.bitsy"};
    if (file_stream.eof()) {
        return 0;
    }

    Lexer lexer{file_stream};
    auto tokens = lexer.get_tokens();
    for (auto token : tokens) {
        token.print();
    }

    file_stream.close();

    return 0;
}
