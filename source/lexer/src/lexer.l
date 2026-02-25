%{
#include <iostream>
#include <string>

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
    token_ = {slr::lexer::TokenType::NUMBER, yytext, yylineno};
    return static_cast<int>(slr::lexer::TokenType::NUMBER); 
}

[a-zA-Z_][a-zA-Z0-9_]* {
    token_ = {slr::lexer::TokenType::ID, yytext, yylineno};
    return static_cast<int>(slr::lexer::TokenType::ID);
}

"+"     { 
    token_ = {slr::lexer::TokenType::PLUS, yytext, yylineno};
    return static_cast<int>(slr::lexer::TokenType::PLUS); 
}
"-"     { 
    token_ = {slr::lexer::TokenType::MINUS, yytext, yylineno};
    return static_cast<int>(slr::lexer::TokenType::MINUS); 
}
"*"     { 
    token_ = {slr::lexer::TokenType::MUL, yytext, yylineno};
    return static_cast<int>(slr::lexer::TokenType::MUL); 
}
"/"     { 
    token_ = {slr::lexer::TokenType::DIV, yytext, yylineno};
    return static_cast<int>(slr::lexer::TokenType::DIV); 
}
"("     { 
    token_ = {slr::lexer::TokenType::LBRACKET, yytext, yylineno};
    return static_cast<int>(slr::lexer::TokenType::LBRACKET); 
}
")"     { 
    token_ = {slr::lexer::TokenType::RBRACKET, yytext, yylineno};
    return static_cast<int>(slr::lexer::TokenType::RBRACKET); 
}

[ \t\n]+ { /* игнорируем пробелы */ }

.       {   
    std::cerr << "Lexical error at line " << yylineno 
        << ": unexpected character '" << yytext << "'" << std::endl;
    token_ = {slr::lexer::TokenType::UNKNOWN, yytext, yylineno};
    return static_cast<int>(slr::lexer::TokenType::UNKNOWN); 
}

<<EOF>> {
    token_ = {slr::lexer::TokenType::END_OF_FILE, "$", yylineno};
    return static_cast<int>(slr::lexer::TokenType::END_OF_FILE);
}

%%