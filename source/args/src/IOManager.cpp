#include "args/IOManager.hpp"

#include "utils/common.hpp"
#include <iostream>

namespace slr {
namespace args {
    
IOManger::IOManger(
    std::optional<std::string> input_filename, 
    std::optional<std::string> output_filename
)   :   input_filename_(input_filename)
    ,   output_filename_(output_filename)
{
    if (input_filename_.has_value()) {
        input_file_.open(*input_filename_);
        if (!input_file_.is_open()) {
            utils::THROW("Can't open input file '" + *input_filename_ + "'");
        }
    }

    if (output_filename_.has_value()) {
        output_file_.open(*output_filename_);
        if (!output_file_.is_open()) {
            input_file_.close();
            utils::THROW("Can't open output file '" + *output_filename_ + "'");
        }
    }
}

IOManger::~IOManger() {
    if (input_file_.is_open()) {
        input_file_.close();
    }
    if (output_file_.is_open()) {
        output_file_.close();
    }
}

std::optional<std::string> IOManger::getInputFilename    ()  const noexcept {return  input_filename_; }
std::optional<std::string> IOManger::getOutputFilename   ()  const noexcept {return output_filename_; }
std::istream&              IOManger::getInputFile        ()        noexcept {return  input_file_.is_open() ?  input_file_ : std::cin;  }
std::ostream&              IOManger::getOutputFile       ()        noexcept {return output_file_.is_open() ? output_file_ : std::cout; }

}
}