#include <fstream>
#include <iostream>

#include "lexer/Lexer.hpp"
#include "lexer/TokenType.hpp"
#include "parser/Parser.hpp"

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

    Parser parser{tokens};
    auto main_block = parser.parse();

    main_block->print();

    return 0;
}
