#pragma once

#include <string>
#include <optional>

namespace slr {
namespace args {

class Args {

public:

    static Args parse(int argc, char* argv[]);
           void print_help() const;

    inline std::string_view getProgramName() const {return program_name_;}
    inline std::optional<std::string_view> getInputFile() const {return input_file_;}
    inline bool getHelp() const {return help_;}
    inline bool getVerbose() const {return verbose_;}

private:

    std::string program_name_;
    std::optional<std::string> input_file_ = std::nullopt;
    bool help_ = false;
    bool verbose_ = false;
};

}
}