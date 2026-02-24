#pragma once

#include <concepts>
#include <variant>
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

namespace slr {
namespace lexer {

template<typename T>
concept TokenVal = std::convertible_to<T, double> || std::convertible_to<T, std::string>;

class Lexer : public yyFlexLexer {

public:

    inline Lexer(std::istream* in = nullptr, std::ostream* out = nullptr) 
        : yyFlexLexer(in, out), last_value_(0.0) {}
    
    virtual int yylex() override;
    
    template <TokenVal T>
    inline T get_token_value() const { return std::get<T>(last_value_); }

private:

    std::variant<double, std::string> last_value_;
};

}
}