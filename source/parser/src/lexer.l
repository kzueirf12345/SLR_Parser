%{
#include "parser/tokens.hpp"
#include "parser/Lexer.hpp"

%}

%option c++
%option header-file="lexer.hpp"
%option yyclass="Lexer"
%option noyywrap 

%%

[0-9]+(\.[0-9]+)? { 
    last_value_ = std::stod(yytext); 
    return static_cast<int>(TokenType::NUMBER); 
}

"+"     { return static_cast<int>(TokenType::PLUS); }
"-"     { return static_cast<int>(TokenType::MINUS); }
"*"     { return static_cast<int>(TokenType::MUL); }
"/"     { return static_cast<int>(TokenType::DIV); }
"("     { return static_cast<int>(TokenType::LBRAKET); }
")"     { return static_cast<int>(TokenType::RBRAKET); }

[ \t\n]+ { /* игнорируем пробелы */ }

.       { return static_cast<int>(TokenType::UNKNOWN); }

%%
