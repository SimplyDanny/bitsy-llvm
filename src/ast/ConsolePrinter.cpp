#include "ast/ConsolePrinter.hpp"

void ConsolePrinter::add_endl() {
    std::cout << std::endl;
    last_was_endl = true;
}

void ConsolePrinter::indented(const NestedPrinter& nested_printer) {
    ++indent;
    nested_printer();
    --indent;
}

ConsolePrinter cout{};
