#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <optional>

namespace slr {
namespace args {

class Args {

public:

    Args(const int argc, const char* const argv[]);

    void                            printHelp           (std::ostream& out = std::cout) const;

    std::string_view                getProgramName      ()                              const noexcept;
    std::optional<std::string>      getInputFilename    ()                              const noexcept;
    std::optional<std::string>      getOutputFilename   ()                              const noexcept;
    bool                            getHelp             ()                              const noexcept;
    bool                            getVerbose          ()                              const noexcept;

private:

    std::string program_name_;
    std::optional<std::string> input_filename_ = std::nullopt;
    std::optional<std::string> output_filename_ = std::nullopt;
    bool help_ = false;
    bool verbose_ = false;
};

}
}