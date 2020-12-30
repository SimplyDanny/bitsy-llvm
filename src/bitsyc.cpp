#include <fstream>
#include <iostream>
#include <iterator>

#include "ast/ASTPrinter.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/ModuleBuilder.hpp"
#include "execution/ModuleProcessor.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Call the program with exactly one argument which is a path to a '.bitsy' file." << std::endl;
        return 1;
    }

    std::ifstream file_stream{argv[1]};
    if (!file_stream.good()) {
        std::cerr << "Cannot open the input file." << std::endl;
        return 2;
    }

    Lexer<std::istreambuf_iterator<char>> lexer{file_stream, {}};
    std::vector<Token> tokens{lexer, decltype(lexer)()};

    Parser parser{tokens};
    auto main_block = parser.parse();

    ModuleBuilder builder{main_block.get()};

    ModuleProcessor processor{builder.build()};
    if (processor.verify()) {
        return 3;
    }
    processor.compile();

    return processor.execute();
}
