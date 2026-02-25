#pragma once

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
    
    static std::string getTokenStr(const Token& token);

    const std::vector<Token>& parse();

private:

    const Token& getToken();

    virtual int yylex() override;

private:

    Token token_;

    std::vector<Token> tokens_;
};

}
}