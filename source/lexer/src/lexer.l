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

[a-zA-Z_][a-zA-Z0-9_]* {
    last_value_ = yytext;
    return static_cast<int>(slr::lexer::TokenType::ID);
}

"+"     { return static_cast<int>(slr::lexer::TokenType::PLUS); }
"-"     { return static_cast<int>(slr::lexer::TokenType::MINUS); }
"*"     { return static_cast<int>(slr::lexer::TokenType::MUL); }
"/"     { return static_cast<int>(slr::lexer::TokenType::DIV); }
"("     { return static_cast<int>(slr::lexer::TokenType::LBRACKET); }
")"     { return static_cast<int>(slr::lexer::TokenType::RBRACKET); }

[ \t\n]+ { /* игнорируем пробелы */ }

.       {   
    std::cerr << "Lexical error at line " << yylineno 
        << ": unexpected character '" << yytext << "'" << std::endl;
    return static_cast<int>(slr::lexer::TokenType::UNKNOWN); 
}

%%
