#include "syntaxer/Grammar.hpp"

#include "lexer/tokens.hpp"
#include <iostream>

namespace slr {
namespace syntaxer {

Grammar::Grammar() {
    
    productions_ = {
        {Symbol::NT_START,  {Symbol::NT_SUM},                                       "<start> -> <sum>"},
        {Symbol::NT_SUM,    {Symbol::NT_SUM, Symbol::PLUS, Symbol::NT_MUL},         "<sum> -> <sum> '+' <mul>"},
        {Symbol::NT_SUM,    {Symbol::NT_SUM, Symbol::MINUS, Symbol::NT_MUL},        "<sum> -> <sum> '-' <mul>"},
        {Symbol::NT_SUM,    {Symbol::NT_MUL},                                       "<sum> -> <mul>"},
        {Symbol::NT_MUL,    {Symbol::NT_MUL, Symbol::MUL, Symbol::NT_BRAKETS},      "<mul> -> <mul> '*' <brakets>"},
        {Symbol::NT_MUL,    {Symbol::NT_MUL, Symbol::DIV, Symbol::NT_BRAKETS},      "<mul> -> <mul> '/' <brakets>"},
        {Symbol::NT_MUL,    {Symbol::NT_BRAKETS},                                   "<mul> -> <brakets>"},
        {Symbol::NT_BRAKETS,{Symbol::LBRACKET, Symbol::NT_SUM, Symbol::RBRACKET},   "<brakets> -> '(' <sum> ')'"},
        {Symbol::NT_BRAKETS,{Symbol::NUMBER},                                       "<brakets> -> NUM"},
        {Symbol::NT_BRAKETS,{Symbol::ID},                                           "<brakets> -> ID"},
    };
    
    buildFirstSets();
    buildFollowSets();
}

void Grammar::buildFollowSets() {
    follow_sets_.resize(static_cast<size_t>(Symbol::COUNT));
    
    follow_sets_[static_cast<size_t>(Symbol::NT_START)].insert(Symbol::END_OF_FILE);
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (const auto& prod : productions_) {
            const auto& body = prod.body;
            
            for (size_t sym_ind = 0; sym_ind < body.size(); ++sym_ind) {
                if (isTerminal(body[sym_ind])) {
                    continue;
                }
                
                std::vector<Symbol> to_add;
                
                if (sym_ind == body.size() - 1) {
                    const auto& follow_head = follow_sets_[static_cast<size_t>(prod.head)];
                    to_add.insert(to_add.end(), follow_head.begin(), follow_head.end());
                }
                else {
                    Symbol next_sym = body[sym_ind + 1];
                    
                    if (isTerminal(next_sym)) {
                        to_add.push_back(next_sym);
                    }
                    else {
                        const auto& first_next = first_sets_[static_cast<size_t>(next_sym)];
                        to_add.insert(to_add.end(), first_next.begin(), first_next.end());
                    }
                }

                auto& follow_current = follow_sets_[static_cast<size_t>(body[sym_ind])];
                const size_t prev_size = follow_current.size(); 

                follow_current.insert(to_add.begin(), to_add.end());
                changed = (prev_size != follow_current.size());
            }
        }
    }
}

void Grammar::buildFirstSets() {
    first_sets_.resize(static_cast<size_t>(Symbol::COUNT));
    
    for (int term_sym = 1; term_sym < static_cast<int>(slr::lexer::TokenType::COUNT); ++term_sym) {
        Symbol s = static_cast<Symbol>(term_sym);
        first_sets_[term_sym].insert(s);
    }
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (const auto& prod : productions_) {
            Symbol first_body = prod.body[0];
            auto& first_head = first_sets_[static_cast<size_t>(prod.head)];
            
            const auto& first_body_set = first_sets_[static_cast<size_t>(first_body)];
            for (Symbol first_body_sym : first_body_set) {
                if (first_head.insert(first_body_sym).second) {
                    changed = true;
                }
            }
        }
    }
}

void Grammar::print(std::ostream& out) const {
    out << "\n=== Grammar Info ===\n";
    out << "\n=== FIRST Sets ===\n";
    for (int i = 0; i < static_cast<int>(Symbol::COUNT); ++i) {
        Symbol sym = static_cast<Symbol>(i);

        out << "FIRST(" << Grammar::getSymbolStr(sym) << ") = { ";
        for (auto sym : getFirst(sym)) {
            out << "'"<< Grammar::getSymbolStr(sym) << "' ";
        }
        out << "}\n";
        
    }
    
    out << "\n=== FOLLOW Sets ===\n";
    for (int i = 0; i < static_cast<int>(Symbol::COUNT); ++i) {
        Symbol sym = static_cast<Symbol>(i);

        out << "FOLLOW(" << Grammar::getSymbolStr(sym) << ") = { ";
        for (auto follow_sym : getFollow(sym)) {
            out << Grammar::getSymbolStr(follow_sym) << " ";
        }
        out << "}\n";
        
    }
    out << "\n";
}

const std::vector<Production>&  Grammar::getProductions() const noexcept { 
    return productions_; 
}

size_t Grammar::getProductionCount() const noexcept { 
    return productions_.size(); 
}

std::string Grammar::productionString(size_t prod_ind) const noexcept { 
    return productions_[prod_ind].name; 
}

const std::unordered_set<Symbol>& Grammar::getFollow(Symbol non_terminal) const {
    return follow_sets_[static_cast<size_t>(non_terminal)];
}
const std::unordered_set<Symbol>& Grammar::getFirst(Symbol symbol) const {
    return first_sets_[static_cast<size_t>(symbol)];
}

bool Grammar::isNonTerminal(Symbol s) {
    return s >= Symbol::NT_START && s < Symbol::COUNT;
}

bool Grammar::isTerminal(Symbol s) {
    return s > Symbol::UNKNOWN && s < static_cast<Symbol>(lexer::TokenType::COUNT);
}

Symbol Grammar::fromTokenType(lexer::TokenType token_type) {
    if (isTerminal(static_cast<Symbol>(token_type))) {
        return static_cast<Symbol>(token_type);
    }
    return Symbol::UNKNOWN;
}


#define CASE_RET_STR(type) case Symbol::type: { return #type; }
std::string Grammar::getSymbolStr(Symbol s) {
    switch(s) {
        CASE_RET_STR(END_OF_FILE)
        CASE_RET_STR(NUMBER)
        CASE_RET_STR(ID)
        CASE_RET_STR(PLUS)
        CASE_RET_STR(MINUS)
        CASE_RET_STR(MUL)
        CASE_RET_STR(DIV)
        CASE_RET_STR(LBRACKET)
        CASE_RET_STR(RBRACKET)
        CASE_RET_STR(NT_START)
        CASE_RET_STR(NT_SUM)
        CASE_RET_STR(NT_MUL)
        CASE_RET_STR(NT_BRAKETS)
        default: {
            return "UNKNOWN";
        }
    }
    return "UNKNOWN";
}
#undef CASE_RET_STR

#define CASE_RET_STR(type, ret) case Symbol::type: { return ret; }
std::string Grammar::getPrettySymbolStr(Symbol s) {
    switch(s) {
        CASE_RET_STR(END_OF_FILE, "$")
        CASE_RET_STR(NUMBER, "NUM")
        CASE_RET_STR(ID, "ID")
        CASE_RET_STR(PLUS, "+")
        CASE_RET_STR(MINUS, "-")
        CASE_RET_STR(MUL, "*")
        CASE_RET_STR(DIV, "-")
        CASE_RET_STR(LBRACKET, "(")
        CASE_RET_STR(RBRACKET, ")")
        CASE_RET_STR(NT_START, "<start>")
        CASE_RET_STR(NT_SUM, "<sum>")
        CASE_RET_STR(NT_MUL, "<mul>")
        CASE_RET_STR(NT_BRAKETS, "<brackets>")
        default: {
            return "UNKNOWN";
        }
    }
    return "UNKNOWN";
}
#undef CASE_RET_STR

}
}