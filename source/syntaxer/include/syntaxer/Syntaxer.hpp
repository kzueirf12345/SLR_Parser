#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <string>

#include "syntaxer/Grammar.hpp"
#include "syntaxer/ParsingTable.hpp"
#include "lexer/tokens.hpp"

namespace slr {
namespace syntaxer {

struct ParseStep {
    std::string stack;
    std::string input;
    std::string action;
};

struct ParseResult {
    std::vector<ParseStep> steps;
    std::optional<std::string> error_message;
};

class Syntaxer {
public:
    Syntaxer(const Grammar& grammar, const ParsingTable& table);
    
    ParseResult parse(const std::vector<lexer::Token>& tokens);
    
    void print(const ParseResult& result, std::ostream& out = std::cout) const;
    
private:
    const Grammar& grammar_;
    const ParsingTable& table_;
    
    std::vector<int> state_stack_;
    std::vector<std::pair<Symbol, std::string>> symbol_stack_;
    
    std::string stackToString() const;
    std::string inputToString(const std::vector<lexer::Token>& tokens, size_t pos) const;
    std::string actionToString(const ParseAction& action) const;
    
    bool doReduce(const ParseAction& action);
    
    bool doShift(
        const ParseAction& action, const std::vector<lexer::Token>& tokens, size_t& token_pos
    );
};

}
}