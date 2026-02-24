#include "utils/common.hpp"
#include "utils/concole.hpp"

#include <stdexcept>

namespace slr {
namespace utils {

void THROW(const std::string& msg, std::source_location loc) {
    throw std::runtime_error(
        std::string(loc.file_name()) + ":" + std::to_string(loc.line()) + "\n"
        "In " + loc.function_name() + ":\n" + 
        YELLOW_FORMAT + msg + NORMAL_FORMAT
    );
}

}
}