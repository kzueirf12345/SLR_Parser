%{
#include <iostream>

#include "lexer/tokens.hpp"
#include "lexer/Lexer.hpp"

%}

%option c++
%option header-file="lexer.hpp"
%option yyclass="slr::lexer::Lexer"
%option noyywrap 
%option yylineno 

%%

[0-9]+(\.[0-9]+)? { 
    last_value_ = std::stod(yytext); 
    return static_cast<int>(slr::lexer::TokenType::NUMBER); 
}

"+"     { return static_cast<int>(slr::lexer::TokenType::PLUS); }
"-"     { return static_cast<int>(slr::lexer::TokenType::MINUS); }
"*"     { return static_cast<int>(slr::lexer::TokenType::MUL); }
"/"     { return static_cast<int>(slr::lexer::TokenType::DIV); }
"("     { return static_cast<int>(slr::lexer::TokenType::LBRAKET); }
")"     { return static_cast<int>(slr::lexer::TokenType::RBRAKET); }

[ \t\n]+ { /* игнорируем пробелы */ }

.       {   
            std::cerr << "Lexical error at line " << yylineno 
                << ": unexpected character '" << yytext << "'" << std::endl;
            return static_cast<int>(slr::lexer::TokenType::UNKNOWN); 
        }

%%
