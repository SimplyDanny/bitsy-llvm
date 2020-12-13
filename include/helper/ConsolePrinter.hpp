#ifndef CONSOLEPRINTER_HPP
#define CONSOLEPRINTER_HPP

#include <functional>
#include <iostream>
#include <type_traits>

class ConsolePrinter;

using NestedPrinter = std::function<void()>;
using EndlPrinter = std::function<void(ConsolePrinter&)>;

template <class T>
inline constexpr bool is_printer = std::is_convertible_v<T, NestedPrinter> || std::is_convertible_v<T, EndlPrinter>;

class ConsolePrinter {
    unsigned int indent = 0;
    bool last_was_endl = true;

  public:
    template <class T, std::enable_if_t<!is_printer<T>, bool> = true>
    ConsolePrinter& operator<<(T output);
    ConsolePrinter& operator<<(const NestedPrinter& nested_printer);
    ConsolePrinter& operator<<(const EndlPrinter& endl_printer);

    friend void endl(ConsolePrinter& printer);
};

template <class T, std::enable_if_t<!is_printer<T>, bool>>
ConsolePrinter& ConsolePrinter::operator<<(const T output) {
    if (last_was_endl) {
        std::cout << std::string(2 * indent, ' ');
    }
    std::cout << output;
    last_was_endl = false;
    return *this;
};

void endl(ConsolePrinter& printer);

extern ConsolePrinter cout;

#endif
