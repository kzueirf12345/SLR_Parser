#include "args/Args.hpp"

#include <iostream>
#include <cstring>

namespace slr {
namespace args {

Args Args::parse(int argc, char* argv[]) {
    Args args;

    args.program_name_ = argv[0];

    for (size_t arg_ind = 1; arg_ind < static_cast<size_t>(argc); ++arg_ind) {
        std::string_view arg = argv[arg_ind];

        if (arg == "-h" || arg == "--help") {
            args.help_ = true;
        }
        else if (arg == "-v" || arg == "--verbose") {
            args.verbose_ = true;
        }
        else if (arg == "-i" || arg == "--input") {
            if (arg_ind + 1 < static_cast<size_t>(argc)) {
                args.input_file_ = argv[++arg_ind];
            }
            else {
                std::cerr << "Error: --input requires an argument\n";
                args.help_ = true;
            }
        }
        else {
            std::cerr << "Error: Unknown option '" << arg << "'\n";
            args.help_ = true;
        }
    }

    return args;
}

void Args::print_help() const {
    std::cout << "Usage: " << program_name_ << " [OPTIONS]\n\n"
              << "Options:\n"
              << "  -h, --help           Show this help message\n"
              << "  -v, --verbose        Enable verbose output\n"
              << "  -i, --input <FILE>   Specify input file\n";
}

}
}