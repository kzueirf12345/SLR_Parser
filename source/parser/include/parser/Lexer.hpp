#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

class Lexer : public yyFlexLexer {
public:
    Lexer(std::istream* in = nullptr, std::ostream* out = nullptr) 
        : yyFlexLexer(in, out), last_value_(0.0) {}
    
    virtual int yylex();
    
    double get_token_value() const { return last_value_; }

private:
    double last_value_;
};