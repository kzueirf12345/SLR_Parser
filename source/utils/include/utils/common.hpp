#pragma once

#include <source_location>
#include <string>

namespace slr {
namespace utils {

void THROW(const std::string& msg, std::source_location loc = std::source_location::current());

}
}