#include "syntaxer/Syntaxer.hpp"

#include <iomanip>
#include <sstream>
#include <algorithm>
#include <string>

#include "syntaxer/ParsingTable.hpp"
#include "lexer/Lexer.hpp"

namespace slr {
namespace syntaxer {

Syntaxer::Syntaxer(const Grammar& grammar, const ParsingTable& table)
    : grammar_(grammar), table_(table) 
{}

ParseResult Syntaxer::parse(const std::vector<lexer::Token>& tokens) {
    ParseResult result;
    result.error_message = std::nullopt;
    
    state_stack_.clear();
    symbol_stack_.clear();
    
    state_stack_.push_back(0);
    symbol_stack_.push_back(Symbol::END_OF_FILE);
    
    size_t token_pos = 0;
    
    while (true) {
        ParseStep step;
        step.stack = stackToString();
        step.input = inputToString(tokens, token_pos);
        
        int current_state = state_stack_.back();
        Symbol lookahead = Grammar::fromTokenType(tokens[token_pos].type);
        
        ParseAction action = table_.getAction(current_state, lookahead);
        step.action = actionToString(action);
        result.steps.push_back(step);
        
        switch (action.type) {
            case ActionType::SHIFT: {
                if (!doShift(action, tokens, token_pos)) {
                    result.error_message = "Shift failed. Line " + std::to_string(tokens[token_pos].line);
                    return result;
                }
                break;
            }
            
            case ActionType::REDUCE: {
                if (!doReduce(action)) {
                    result.error_message = "Reduce failed. Line " + std::to_string(tokens[token_pos].line);
                    return result;
                }
                break;
            }
            
            case ActionType::ACCEPT: {
                return result;
            }
            
            case ActionType::ERROR: {
                std::ostringstream oss;
                oss << "Syntax error in line " << tokens[token_pos].line << ", token is " 
                    << lexer::Lexer::getTokenStr(tokens[token_pos])
                    << "(state " << current_state << ")";
                result.error_message = oss.str();
                return result;
            }

            default: {
                result.error_message = "Unknown action type. Line " + std::to_string(tokens[token_pos].line);
                return result;
            }
        }
    }
}

bool Syntaxer::doShift(
    const ParseAction& action, const std::vector<lexer::Token>& tokens, size_t& token_pos
) {
    Symbol sym = Grammar::fromTokenType(tokens[token_pos].type);
    
    symbol_stack_.push_back(sym);
    state_stack_.push_back(action.target);
    ++token_pos;
    
    return true;
}

bool Syntaxer::doReduce(const ParseAction& action) {
    const auto& prod = grammar_.getProductions()[action.target];
    
    size_t body_size = prod.body.size();
    for (size_t i = 0; i < body_size; ++i) {
        state_stack_.pop_back();
        symbol_stack_.pop_back();
    }
    
    symbol_stack_.push_back(prod.head);
    
    int prev_state = state_stack_.back();
    auto goto_state = table_.getGoto(prev_state, prod.head);
    
    if (!goto_state) {
        return false;
    }
    
    state_stack_.push_back(*goto_state);
    
    return true;
}

std::string Syntaxer::stackToString() const {
    std::ostringstream oss;
    
    for (size_t i = 0; i < symbol_stack_.size(); ++i) {
        oss << Grammar::symbolName(symbol_stack_[i]) << "";
    }
    
    return oss.str();
}

std::string Syntaxer::inputToString(const std::vector<lexer::Token>& tokens, size_t pos) const {
    std::ostringstream oss;
    
    for (size_t i = pos; i < tokens.size(); ++i) {
        oss << lexer::Lexer::getTokenStr(tokens[i]) << " ";
    }
    
    return oss.str();
}

std::string Syntaxer::actionToString(const ParseAction& action) const {
    std::ostringstream oss;
    
    switch (action.type) {
        case ActionType::SHIFT:
            oss << "shift " << action.target;
            break;
            
        case ActionType::REDUCE: {
            const auto& prod = grammar_.getProductions()[action.target];
            oss << "reduce " << prod.name;
            break;
        }
            
        case ActionType::ACCEPT:
            oss << "accept";
            break;
            
        case ActionType::ERROR:
            oss << "error";
            break;
    }
    
    return oss.str();
}

void Syntaxer::print(const ParseResult& result) const {

    std::cout << "\n=== SLR Parse result ===\n";
    std::cout << "Result: ";

    if (!result.error_message.has_value()) {
        std::cout << "SUCCESSFULLY\n\n";
    }
    else {
        std::cout << "ERROR\n" << "Message: " << *result.error_message << "\n";
    }

    std::cout << std::left 
              << std::setw(50) << "STACK" 
              << std::setw(50) << "INPUT" 
              << "ACTION" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    for (const auto& step : result.steps) {
        std::cout << std::left 
                  << std::setw(50) << step.stack 
                  << std::setw(50) << step.input 
                  << step.action << std::endl;
    }
    
    std::cout << std::string(70, '-') << std::endl;
}

}
}