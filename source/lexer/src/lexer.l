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
    token_.type = slr::lexer::TokenType::NUMBER;
    token_.value = std::stod(yytext); 
    token_.line = yylineno;
    return static_cast<int>(slr::lexer::TokenType::NUMBER); 
}

[a-zA-Z_][a-zA-Z0-9_]* {
    token_.type = slr::lexer::TokenType::ID;
    token_.value = std::string(yytext);
    token_.line = yylineno;
    return static_cast<int>(slr::lexer::TokenType::ID);
}

"+"     { 
    token_.type = slr::lexer::TokenType::PLUS;
    token_.line = yylineno;
    return static_cast<int>(slr::lexer::TokenType::PLUS); 
}
"-"     { 
    token_.type = slr::lexer::TokenType::MINUS;
    token_.line = yylineno;
    return static_cast<int>(slr::lexer::TokenType::MINUS); 
}
"*"     { 
    token_.type = slr::lexer::TokenType::MUL;
    token_.line = yylineno;
    return static_cast<int>(slr::lexer::TokenType::MUL); 
}
"/"     { 
    token_.type = slr::lexer::TokenType::DIV;
    token_.line = yylineno;
    return static_cast<int>(slr::lexer::TokenType::DIV); 
}
"("     { 
    token_.type = slr::lexer::TokenType::LBRACKET;
    token_.line = yylineno;
    return static_cast<int>(slr::lexer::TokenType::LBRACKET); 
}
")"     { 
    token_.type = slr::lexer::TokenType::RBRACKET;
    token_.line = yylineno;
    return static_cast<int>(slr::lexer::TokenType::RBRACKET); 
}

[ \t\n]+ { /* игнорируем пробелы */ }

.       {   
    std::cerr << "Lexical error at line " << yylineno 
        << ": unexpected character '" << yytext << "'" << std::endl;
    token_.type = slr::lexer::TokenType::UNKNOWN;
    token_.line = yylineno;
    return static_cast<int>(slr::lexer::TokenType::UNKNOWN); 
}

<<EOF>> {
    token_.type = slr::lexer::TokenType::END_OF_FILE;
    token_.line = yylineno;
    return static_cast<int>(slr::lexer::TokenType::END_OF_FILE);
}

%%