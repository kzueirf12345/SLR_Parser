#pragma once

#include <fstream>
#include <string>
#include <optional>

namespace slr {
namespace args {

class IOManger {

public:

    IOManger(
        std::optional<std::string> input_filename = std::nullopt, 
        std::optional<std::string> output_filename = std::nullopt
    );

    IOManger(const IOManger& that) = delete;
    IOManger(const IOManger&& that) = delete;

    IOManger& operator=(const IOManger& that) = delete;
    IOManger& operator=(const IOManger&& that) = delete;

    ~IOManger();

    std::optional<std::string> getInputFilename    ()  const noexcept;
    std::optional<std::string> getOutputFilename   ()  const noexcept;
    std::istream&              getInputFile        ()        noexcept;
    std::ostream&              getOutputFile       ()        noexcept;

private:

    std::optional<std::string> input_filename_  = std::nullopt;
    std::optional<std::string> output_filename_ = std::nullopt;
    std::ifstream              input_file_                    ;
    std::ofstream              output_file_                   ;
};

}
}