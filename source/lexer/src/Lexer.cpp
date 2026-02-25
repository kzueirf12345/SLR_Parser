#include "lexer/Lexer.hpp"

#include "lexer/tokens.hpp"
#include <iomanip>

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

void Lexer::print(std::ostream& out) const {
    out << "\n=== Lexer ===\n";
    out << std::left << std::setw(30) << "TYPE" << "VALUE" << "\n";
    out << std::string(30, '-') << "\n";
    
    for (const auto& token : tokens_) {
        out << std::left << std::setw(30) 
            << Lexer::getTokenTypeStr(token.type) << token.value << "\n";
    }
    
    out << std::string(30, '-') << "\n";
    out << "Total tokens: " << tokens_.size() << "\n";
}

#define CASE_RET_STR(type) case TokenType::type: { return #type; }
std::string Lexer::getTokenTypeStr(TokenType tt) {
    switch (tt) {
        CASE_RET_STR(END_OF_FILE)
        CASE_RET_STR(PLUS)
        CASE_RET_STR(MINUS)
        CASE_RET_STR(MUL)
        CASE_RET_STR(DIV)
        CASE_RET_STR(LBRACKET)
        CASE_RET_STR(RBRACKET)
        CASE_RET_STR(ID)
        CASE_RET_STR(NUMBER)
        default: {
            return "UNKNOWN";
        }
    }
    return "UNKNOWN";
}
#undef CASE_RET_STR

}
}