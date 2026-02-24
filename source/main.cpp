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

    // int token;
    // while ((token = lexer.yylex()) != 0) {
    //     switch (static_cast<slr::lexer::TokenType>(token)) {
    //         case slr::lexer::TokenType::NUMBER:
    //             std::cout << "NUMBER: " << lexer.get_token_value() << std::endl;
    //             break;
    //         case slr::lexer::TokenType::PLUS:
    //             std::cout << "OP: +" << std::endl;
    //             break;
    //         case slr::lexer::TokenType::MINUS:
    //             std::cout << "OP: -" << std::endl;
    //             break;
    //         case slr::lexer::TokenType::MUL:
    //             std::cout << "OP: *" << std::endl;
    //             break;
    //         case slr::lexer::TokenType::DIV:
    //             std::cout << "OP: /" << std::endl;
    //             break;
    //         case slr::lexer::TokenType::LBRAKET:
    //             std::cout << "SYM: (" << std::endl;
    //             break;
    //         case slr::lexer::TokenType::RBRAKET:
    //             std::cout << "SYM: )" << std::endl;
    //             break;
    //         default:
    //             return EXIT_FAILURE;
    //     }
    // }

    // if (args.getVerbose()) {
    //     std::cout << "Parsing complete\n";
    // }

    slr::syntaxer::Grammar g;
    
    // FOLLOW(Start) = {$}
    assert(g.getFollow(slr::syntaxer::Symbol::NT_START).size() == 1);
    assert(g.getFollow(slr::syntaxer::Symbol::NT_START).count(slr::syntaxer::Symbol::END_OF_FILE) == 1);
    
    // FOLLOW(sum) = {$, ), +, -}
    const auto& follow_expr = g.getFollow(slr::syntaxer::Symbol::NT_SUM);
    assert(follow_expr.size() == 4);
    assert(follow_expr.count(slr::syntaxer::Symbol::END_OF_FILE) == 1);
    assert(follow_expr.count(slr::syntaxer::Symbol::RBRACKET) == 1);
    assert(follow_expr.count(slr::syntaxer::Symbol::PLUS) == 1);
    assert(follow_expr.count(slr::syntaxer::Symbol::MINUS) == 1);
    
    // FOLLOW(mul) = {$, ), +, -, *, /}
    const auto& follow_term = g.getFollow(slr::syntaxer::Symbol::NT_MUL);
    assert(follow_term.size() == 6);
    assert(follow_term.count(slr::syntaxer::Symbol::MUL) == 1);
    assert(follow_term.count(slr::syntaxer::Symbol::DIV) == 1);
    
    // FOLLOW(brackets) = FOLLOW(mul)
    const auto& follow_factor = g.getFollow(slr::syntaxer::Symbol::NT_BRAKETS);
    assert(follow_factor == follow_term);
    
    std::cout << "All FOLLOW set tests passed!\n";

    return 0;
}

/*!SECTION

<start>  ::= <sum>

<sum>    ::= <sum> "+" <mul>
           | <sum> "-" <mul>
           | <mul>

<mul>    ::= <mul> "*" <brakets>
           | <mul> "/" <brakets>
           | <brakets>

<brakets> ::= "(" <sum> ")"
           | NUM
*/