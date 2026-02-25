#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cassert>

#include "lexer/tokens.hpp"
#include "lexer/Lexer.hpp"
#include "args/Args.hpp"
#include "syntaxer/Grammar.hpp"
#include "syntaxer/ParsingTable.hpp"
#include "syntaxer/Syntaxer.hpp"
#include "utils/concole.hpp"

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
    slr::syntaxer::Grammar grammar;
    slr::syntaxer::ParsingTable parsing_table(grammar);

    if (args.getVerbose()) {
        grammar.print();
    }

    if (args.getVerbose()) {
        parsing_table.print();
    }

    const auto& tokens = lexer.parse();

    slr::syntaxer::Syntaxer syntaxer(grammar, parsing_table);

    const auto& parse_result = syntaxer.parse(tokens);

    if (args.getVerbose()) {
        syntaxer.print(parse_result);
    }

    if (args.getVerbose()) {
        std::cout << "Parsing complete\n";
    }

    return EXIT_SUCCESS;
}