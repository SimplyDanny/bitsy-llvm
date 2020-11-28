#include <fstream>
#include <iostream>
#include <iterator>

#include "ast/ASTPrinter.hpp"
#include "codegen/CodeGenerator.hpp"
#include "helper/PostProcessor.hpp"
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

    Lexer lexer{std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>()};
    auto tokens = lexer.get_tokens();

    Parser parser{tokens};
    auto main_block = parser.parse();

    CodeGenerator generator{[](auto module) {
        ClangCompiler().process(module);
        Executor().process(module);
    }};
    generator.visit(static_cast<Statement*>(main_block.get()));

    return 0;
}
