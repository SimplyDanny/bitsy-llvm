#include "ast/ASTPrinter.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/ModuleBuilder.hpp"
#include "execution/ModuleProcessor.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"

#include "llvm/Support/CommandLine.h"

#include <fstream>
#include <iostream>
#include <iterator>

namespace cl = llvm::cl;

namespace { namespace opt {

cl::OptionCategory category{"Options"};

cl::opt<std::string> input_name{cl::Positional, cl::Required, cl::desc("<bitsy file>"), cl::cat(category)};
cl::opt<std::string> output_name{"o",
                                 cl::desc("Name of the executable output file"),
                                 cl::value_desc("executable"),
                                 cl::init("a.out"),
                                 cl::cat(category)};
cl::opt<bool> compile{"c", cl::desc("Compile Bitsy file to an exectuable output file"), cl::cat(category)};
cl::opt<bool> quiet{"q", cl::desc("Do not execute the program automatically"), cl::cat(category)};
cl::opt<bool> no_optimization{"no-opt", cl::desc("Do not run any optimization"), cl::cat(category)};
cl::opt<bool> show_cfg{"show-cfg", cl::desc("Show CFG or create an image of it"), cl::cat(category)};
cl::opt<bool> show_ast{"show-ast", cl::desc("Print the internally used AST"), cl::cat(category)};

}} // namespace ::opt

int main(int argc, char *argv[]) {
    cl::HideUnrelatedOptions(opt::category);
    cl::ParseCommandLineOptions(argc, argv, "Compiler for Bitsy programs", nullptr, nullptr, true);

    std::ifstream file_stream{opt::input_name};
    if (!file_stream.good()) {
        std::cerr << "Cannot open the input file." << std::endl;
        return 1;
    }

    Lexer<std::istreambuf_iterator<char>> lexer{file_stream, {}};
    std::vector<Token> tokens{lexer, decltype(lexer)()};

    Parser parser{tokens};
    auto main_block = parser.parse();

    ModuleBuilder builder{main_block.get()};

    ModuleProcessor processor{builder.build(), opt::output_name};
    if (processor.verify()) {
        return 2;
    }
    if (!opt::no_optimization) {
        processor.optimize();
    }
    if (opt::compile) {
        if (processor.compile() != 0) {
            return 3;
        }
    }
    if (opt::show_cfg) {
        if (processor.show_cfg()) {
            return 4;
        }
    }
    if (opt::show_ast) {
        ASTPrinter().visit(llvm::cast<Statement>(main_block.get()));
    }
    if (opt::quiet || opt::show_cfg || opt::show_ast) {
        return 0;
    }
    return processor.execute();
}
