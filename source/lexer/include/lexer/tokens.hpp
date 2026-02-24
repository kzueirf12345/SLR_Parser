#pragma once

namespace slr {
namespace lexer {

enum class TokenType {
    END_OF_FILE = 0,
    NUMBER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LBRAKET,
    RBRAKET,
    UNKNOWN
};

struct Token {
    TokenType type;
    double value;
    int line;
};

}
}