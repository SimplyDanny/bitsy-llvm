#include <fstream>
#include <iostream>

#include "ast/ASTPrinter.hpp"
#include "codegen/CodeGenerator.hpp"
#include "helper/PostProcessor.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Call the program with exactly one argument which is a path to a '.bitsy' file." << std::endl;
        exit(1);
    }

    std::ifstream file_stream{argv[1]};
    if (file_stream.eof()) {
        return 0;
    }

    Lexer lexer{file_stream};
    auto tokens = lexer.get_tokens();

    file_stream.close();

    Parser parser{tokens};
    auto main_block = parser.parse();

    CodeGenerator generator{[](auto module) {
        ClangCompiler().process(module);
        Executor().process(module);
    }};
    generator.visit(static_cast<Statement*>(main_block.get()));

    return 0;
}
