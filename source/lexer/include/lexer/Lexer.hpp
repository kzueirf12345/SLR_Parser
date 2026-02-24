#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

namespace slr {
namespace lexer {

class Lexer : public yyFlexLexer {

public:

    inline Lexer(std::istream* in = nullptr, std::ostream* out = nullptr) 
        : yyFlexLexer(in, out), last_value_(0.0) {}
    
    virtual int yylex() override;
    
    inline double get_token_value() const { return last_value_; }

private:

    double last_value_;
};

}
}