#include "lexer/Lexer.hpp"

#include "lexer/tokens.hpp"

namespace slr {
namespace lexer {

Lexer::Lexer(std::istream* in, std::ostream* out) 
    :   yyFlexLexer(in, out)
{}

const Token& Lexer::getToken() { 
    yylex();
    return token_; 
}

const std::vector<Token>& Lexer::parse() {

    Token token;
    while ((token = getToken()).type != slr::lexer::TokenType::END_OF_FILE) {
        tokens_.push_back(std::move(token));
    }
    tokens_.push_back(std::move(token));

    return tokens_;
}

#define CASE_RET_STR(type, ret) case TokenType::type: { return ret; }
std::string Lexer::getTokenStr(const Token& token) {
    switch (token.type) {
        CASE_RET_STR(END_OF_FILE, "EOF")
        CASE_RET_STR(PLUS, "+")
        CASE_RET_STR(MINUS, "-")
        CASE_RET_STR(MUL, "*")
        CASE_RET_STR(DIV, "/")
        CASE_RET_STR(LBRACKET, "(")
        CASE_RET_STR(RBRACKET, ")")
        case slr::lexer::TokenType::NUMBER: return std::to_string(std::get<double>(token.value));
        case slr::lexer::TokenType::ID: return std::get<std::string>(token.value);
        default: {
            return "UNKNOWN";
        }
    }
    return "UNKNOWN";
}
#undef CASE_RET_STR

}
}