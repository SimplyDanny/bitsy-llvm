#include "lexer/Token.hpp"

#include <iostream>
#include <ostream>

void Token::print() const {
    std::cout << type << "(" << value << ")" << std::endl;
}
