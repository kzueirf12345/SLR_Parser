#include "args/Args.hpp"

#include <iostream>
#include <cstring>

namespace slr {
namespace args {

Args::Args(const int argc, const char* const argv[])
    :   program_name_(argv[0])
{
    for (size_t arg_ind = 1; arg_ind < static_cast<size_t>(argc); ++arg_ind) {
        std::string_view arg = argv[arg_ind];

        if (arg == "-h" || arg == "--help") {
            help_ = true;
        }
        else if (arg == "-v" || arg == "--verbose") {
            verbose_ = true;
        }
        else if (arg == "-i" || arg == "--input") {
            if (arg_ind + 1 < static_cast<size_t>(argc)) {
                input_filename_ = argv[++arg_ind];
            }
            else {
                std::cerr << "Error: --input requires an argument\n";
                help_ = true;
            }
        } else if (arg == "-o" || arg == "--output") {
            if (arg_ind + 1 < static_cast<size_t>(argc)) {
                output_filename_ = argv[++arg_ind];
            }
            else {
                std::cerr << "Error: --output requires an argument\n";
                help_ = true;
            }
        }
        else {
            std::cerr << "Error: Unknown option '" << arg << "'\n";
            help_ = true;
        }
    }
}

void Args::printHelp(std::ostream& out) const {
    out << "Usage: " << program_name_ << " [OPTIONS]\n\n"
           "Options:\n"
           "  -h, --help           Show this help message\n"
           "  -v, --verbose        Enable verbose output\n"
           "  -i, --input <FILE>   Specify input file\n"
           "  -o, --output <FILE>  Specify output file\n";
}

std::string_view             Args::getProgramName      () const noexcept { return program_name_; }
std::optional<std::string>   Args::getInputFilename    () const noexcept { return input_filename_; }
std::optional<std::string>   Args::getOutputFilename   () const noexcept { return output_filename_; }
bool                         Args::getHelp             () const noexcept { return help_; }
bool                         Args::getVerbose          () const noexcept { return verbose_; }

}
}