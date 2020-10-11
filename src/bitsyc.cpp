#include <fstream>

#include "codegen/CodeGenerator.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

int main(int argc, char* argv[]) {
    std::ifstream file_stream{argv[1]};
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

    CodeGenerator generator{std::move(main_block)};
    generator.generate();

    return 0;
}
