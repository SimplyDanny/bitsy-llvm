#ifndef CONSOLEPRINTER_HPP
#define CONSOLEPRINTER_HPP

#include <functional>
#include <iostream>
#include <type_traits>

class ConsolePrinter {
    using NestedPrinter = std::function<void()>;
    using EndlPrinter = std::function<void(ConsolePrinter &)>;

    template <class T>
    static constexpr bool is_printer = std::is_convertible_v<T, NestedPrinter> || std::is_convertible_v<T, EndlPrinter>;

    unsigned int indent = 0;
    bool last_was_endl = true;

  public:
    template <class T>
        requires(!ConsolePrinter::is_printer<T>)
    ConsolePrinter &operator<<(const T &output);
    ConsolePrinter &operator<<(const NestedPrinter &nested_printer);
    ConsolePrinter &operator<<(const EndlPrinter &endl_printer);

    friend void endl(ConsolePrinter &printer);
};

template <class T>
    requires(!ConsolePrinter::is_printer<T>)
ConsolePrinter &ConsolePrinter::operator<<(const T &output) {
    if (last_was_endl) {
        std::cout << std::string(2 * indent, ' ');
    }
    if constexpr (std::is_enum_v<T>) {
        std::cout << static_cast<std::underlying_type_t<T>>(output);
    } else {
        std::cout << output;
    }
    last_was_endl = false;
    return *this;
};

void endl(ConsolePrinter &printer);

extern ConsolePrinter cout;

#endif
