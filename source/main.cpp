#include <iostream>
#include <sstream>
#include "parser/tokens.hpp"
#include "parser/Lexer.hpp"

int main() {
    std::string expression = "2 + 2 * (3 - 1)";
    std::istringstream iss(expression);

    Lexer lexer(&iss);

    std::cout << "Parsing: " << expression << std::endl;

    int token;
    while ((token = lexer.yylex()) != 0) { // 0 = EOF
        switch (static_cast<TokenType>(token)) {
            case TokenType::NUMBER:
                // Получаем значение через метод класса, а не глобальную переменную
                std::cout << "NUMBER: " << lexer.get_token_value() << std::endl;
                break;
            case TokenType::PLUS:
                std::cout << "OP: +" << std::endl;
                break;
            case TokenType::MINUS:
                std::cout << "OP: -" << std::endl;
                break;
            case TokenType::MUL:
                std::cout << "OP: *" << std::endl;
                break;
            case TokenType::DIV:
                std::cout << "OP: /" << std::endl;
                break;
            case TokenType::LBRAKET:
                std::cout << "SYM: (" << std::endl;
                break;
            case TokenType::RBRAKET:
                std::cout << "SYM: )" << std::endl;
                break;
            default:
                std::cout << "UNKNOWN TOKEN: " << token << std::endl;
                break;
        }
    }

    return 0;
}