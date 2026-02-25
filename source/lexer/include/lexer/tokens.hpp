#pragma once

#include <concepts>
#include <string>
#include <variant>

namespace slr {
namespace lexer {

template<typename T>
concept TokenVal = std::convertible_to<T, double> || std::convertible_to<T, std::string>;

enum class TokenType: int {
    END_OF_FILE = 0,
    NUMBER,
    ID,
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
    TokenType type = TokenType::UNKNOWN;
    std::variant<std::string, double> value = 0.;
    int line = -1;
};

}
}