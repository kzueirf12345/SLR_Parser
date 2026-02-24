#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cassert>

#include "lexer/tokens.hpp"
#include "lexer/Lexer.hpp"
#include "args/Args.hpp"
#include "syntaxer/Grammar.hpp"

int main(int argc, char* argv[]) {
    slr::args::Args args = slr::args::Args::parse(argc, argv);

    if (args.getHelp()) {
        args.print_help();
        return EXIT_SUCCESS;
    }

    std::istream* input_stream = nullptr;
    std::ifstream input_fstream;

    if (args.getInputFile().has_value()) {
        const std::string input_filename(args.getInputFile().value());
        input_fstream.open(input_filename);

        if (!input_fstream.is_open()) {
            std::cerr << "Error: Cannot open file '" << input_filename << "'\n";
            return EXIT_FAILURE;
        }
        input_stream = &input_fstream;

        if (args.getVerbose()) {
            std::cout << "Reading from file: " << input_filename << "\n";
        }
    }
    else {
        input_stream = &std::cin;
        if (args.getVerbose()) {
            std::cout << "Reading from stdin (enter expression, then Ctrl+D)\n";
        }
    }

    slr::lexer::Lexer lexer(input_stream);

    int token;
    while ((token = lexer.yylex()) != 0) {
        switch (static_cast<slr::lexer::TokenType>(token)) {
            case slr::lexer::TokenType::NUMBER:
                std::cout << "NUMBER: '" << lexer.get_token_value<double>() << "'" << std::endl;
                break;
            case slr::lexer::TokenType::ID:
                std::cout << "ID: '" << lexer.get_token_value<std::string>() << "'" << std::endl;
                break;
            case slr::lexer::TokenType::PLUS:
                std::cout << "OP: +" << std::endl;
                break;
            case slr::lexer::TokenType::MINUS:
                std::cout << "OP: -" << std::endl;
                break;
            case slr::lexer::TokenType::MUL:
                std::cout << "OP: *" << std::endl;
                break;
            case slr::lexer::TokenType::DIV:
                std::cout << "OP: /" << std::endl;
                break;
            case slr::lexer::TokenType::LBRACKET:
                std::cout << "SYM: (" << std::endl;
                break;
            case slr::lexer::TokenType::RBRACKET:
                std::cout << "SYM: )" << std::endl;
                break;
            default:
                return EXIT_FAILURE;
        }
    }

    if (args.getVerbose()) {
        std::cout << "Parsing complete\n";
    }

    slr::syntaxer::Grammar grammar;

    if (args.getVerbose()) {
        std::cout << "\n=== Grammar Info ===\n";
        std::cout << "\n=== FIRST Sets ===\n";
        for (int i = static_cast<int>(slr::syntaxer::Symbol::NT_START); 
             i < static_cast<int>(slr::syntaxer::Symbol::COUNT); 
             ++i
        ) {
            slr::syntaxer::Symbol s = static_cast<slr::syntaxer::Symbol>(i);
            std::cout << "FIRST(" << slr::syntaxer::Grammar::symbolName(s) << ") = { ";
            for (auto sym : grammar.getFirst(s)) {
                std::cout << slr::syntaxer::Grammar::symbolName(sym) << " ";
            }
            std::cout << "}\n";
            
        }
        
        std::cout << "\n=== FOLLOW Sets ===\n";
        for (int i = static_cast<int>(slr::syntaxer::Symbol::NT_START); 
             i < static_cast<int>(slr::syntaxer::Symbol::COUNT); 
             ++i
        ) {
            slr::syntaxer::Symbol s = static_cast<slr::syntaxer::Symbol>(i);
            std::cout << "FOLLOW(" << slr::syntaxer::Grammar::symbolName(s) << ") = { ";
            for (auto sym : grammar.getFollow(s)) {
                std::cout << slr::syntaxer::Grammar::symbolName(sym) << " ";
            }
            std::cout << "}\n";
            
        }
        std::cout << "\n";
    }

    return 0;
}