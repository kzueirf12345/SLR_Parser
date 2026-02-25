#pragma once

#include <concepts>
#include <string>

namespace slr {
namespace lexer {

template<typename T>
concept TokenVal = std::convertible_to<T, double> || std::convertible_to<T, std::string>;

enum class TokenType: int {
    UNKNOWN = 0,
    END_OF_FILE,
    NUMBER,
    ID,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LBRACKET,
    RBRACKET,
    COUNT
};

struct Token {

    TokenType type = TokenType::UNKNOWN;
    std::string value = "";
    int line = -1;

    Token() = default;

    Token(TokenType type, std::string value, int line)
        :   type(type)
        ,   value(value)
        ,   line(line)
    {}
};

}
}