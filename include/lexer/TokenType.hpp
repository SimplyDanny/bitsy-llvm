#ifndef TOKENTYPE_HPP
#define TOKENTYPE_HPP

enum class TokenType {
    // Blocks
    begin_t,
    end_t,

    // Control flow
    loop_t,
    break_t,
    ifn_t,
    ifp_t,
    ifz_t,
    else_t,

    // Identifiers
    variable_t,
    number_t,

    // Operators
    left_parenthesis_t,
    right_parenthesis_t,
    operator_t,
    assignment_t,

    // Built-ins
    print_t,
    read_t,
};

#endif
