#ifndef CONSOLEPRINTER_HPP
#define CONSOLEPRINTER_HPP

#include <functional>
#include <iostream>

using NestedPrinter = std::function<void()>;

class ConsolePrinter {
    unsigned int indent = 0;
    bool last_was_endl = true;

  public:
    template <class T>
    friend ConsolePrinter& operator<<(ConsolePrinter& printer, const T output);

    void add_endl();
    void indented(const NestedPrinter& nested_printer);
};

template <class T>
ConsolePrinter& operator<<(ConsolePrinter& printer, const T output) {
    if (printer.last_was_endl) {
        std::cout << std::string(2 * printer.indent, ' ');
    }
    std::cout << output;
    printer.last_was_endl = false;
    return printer;
};

extern ConsolePrinter cout;

#endif
