#pragma once

namespace slr {
namespace lexer {

enum class TokenType: int {
    END_OF_FILE = 0,
    NUMBER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LBRACKET,
    RBRACKET,
    UNKNOWN,
    COUNT
};

struct Token {
    TokenType type;
    double value;
    int line;
};

}
}