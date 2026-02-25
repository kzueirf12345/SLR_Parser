#include <cstdlib>
#include <exception>
#include <iostream>
#include <cassert>

#include "args/IOManager.hpp"
#include "args/Args.hpp"
#include "syntaxer/Grammar.hpp"
#include "syntaxer/ParsingTable.hpp"
#include "syntaxer/Syntaxer.hpp"
#include "utils/concole.hpp"
#include "lexer/Lexer.hpp"

int main(int argc, char* argv[]) try {
    const slr::args::Args args(argc, argv);

    if (args.getHelp()) {
        args.printHelp();
        return EXIT_SUCCESS;
    }

    slr::args::IOManger iomanager(args.getInputFilename(), args.getOutputFilename());

    if (args.getVerbose()) {
        std::cout << "Input: " 
            << (iomanager.getInputFilename().has_value() 
            ? iomanager.getInputFilename().value() 
            : "concole (fot end use Ctrl+D)")
            << "\n";
        std::cout << "Output: " 
            << (iomanager.getOutputFilename().has_value() 
            ? iomanager.getOutputFilename().value() 
            : "concole")
            << "\n";
    }

    slr::syntaxer::Grammar grammar;
    slr::syntaxer::ParsingTable parsing_table(grammar);

    if (args.getVerbose()) {
        grammar.print(iomanager.getOutputFile());
        parsing_table.print(iomanager.getOutputFile());
    }

    slr::lexer::Lexer lexer(&iomanager.getInputFile(), &iomanager.getOutputFile());

    const auto& tokens = lexer.parse();

    if (args.getVerbose()) {
        lexer.print(iomanager.getOutputFile());
    }

    slr::syntaxer::Syntaxer syntaxer(grammar, parsing_table);

    const auto& parse_result = syntaxer.parse(tokens);

    if (args.getVerbose()) {
        syntaxer.print(parse_result, iomanager.getOutputFile());
    }

    if (args.getVerbose()) {
        std::cout << "Parsing complete\n";
    }

    return EXIT_SUCCESS;
}
catch(const std::exception& e) {
    std::cerr << RED_FORMAT << "EXCEPTION!!!\n" << e.what() << NORMAL_FORMAT << "\n"; 
    return EXIT_FAILURE;
}
catch(...) {
    std::cerr << RED_FORMAT << "SOMETHING WENT WRONG!!!\n"<< NORMAL_FORMAT << "\n";
    return EXIT_FAILURE;
}