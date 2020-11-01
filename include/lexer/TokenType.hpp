#ifndef TOKENTYPE_HPP
#define TOKENTYPE_HPP

enum TokenType {
    // Blocks
    t_begin,
    t_end,

    // Control flow
    t_loop,
    t_break,
    t_ifn,
    t_ifp,
    t_ifz,
    t_else,

    // Identifiers
    t_variable,
    t_number,

    // Operators
    t_left_parenthesis,
    t_right_parenthesis,
    t_operator,
    t_assignment,

    // Built-ins
    t_print,
    t_read,

    // Ignorables
    t_comment,
    t_whitespace,
};

#endif
