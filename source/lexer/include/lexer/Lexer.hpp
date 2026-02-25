#pragma once

#include <iostream>
#include <vector>

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "lexer/tokens.hpp"

namespace slr {
namespace lexer {

class Lexer : public yyFlexLexer {

public:

    Lexer(std::istream* in = nullptr, std::ostream* out = nullptr);
    
    static std::string getTokenTypeStr(TokenType token);

    const std::vector<Token>& parse();

    void print(std::ostream& out = std::cout) const;

private:

    const Token& getToken();

    virtual int yylex() override;

private:

    Token token_;

    std::vector<Token> tokens_;

};

}
}