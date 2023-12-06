#include "helper/ConsolePrinter.hpp"

ConsolePrinter &ConsolePrinter::operator<<(const NestedPrinter &nested_printer) {
    ++indent;
    nested_printer();
    --indent;
    return *this;
}

ConsolePrinter &ConsolePrinter::operator<<(const EndlPrinter &endl_printer) {
    endl_printer(*this);
    return *this;
}

void endl(ConsolePrinter &printer) {
    std::cout << '\n';
    printer.last_was_endl = true;
}

ConsolePrinter cout;
