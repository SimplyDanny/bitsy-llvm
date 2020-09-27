#include <iostream>
#include <ostream>

#include "lexer/Token.hpp"

void Token::print() const {
    std::cout << type << "(" << value << ")" << std::endl;
}
