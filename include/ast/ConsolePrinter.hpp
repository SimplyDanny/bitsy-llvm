#ifndef CONSOLEPRINTER_HPP
#define CONSOLEPRINTER_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>

typedef std::function<void(void)> NestedPrinter;

class ConsolePrinter {
    unsigned int indent = 0;
    bool last_was_endl = true;

  public:
    template <typename T>
    friend ConsolePrinter& operator<<(ConsolePrinter& printer, const T output) {
        if (printer.last_was_endl) {
            std::cout << std::string(2 * printer.indent, ' ');
        }
        std::cout << output;
        printer.last_was_endl = false;
        return printer;
    };

    void add_endl();
    void indented(const NestedPrinter& nested_printer);
};

extern ConsolePrinter cout;

#endif
