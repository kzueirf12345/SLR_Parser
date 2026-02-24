#include "syntaxer/Grammar.hpp"

#include "lexer/tokens.hpp"

namespace slr::syntaxer {

Grammar::Grammar() {
    
    productions_ = {
        {Symbol::NT_START,  {Symbol::NT_SUM},                                   "<start> -> <sum>"},
        {Symbol::NT_SUM,    {Symbol::NT_SUM, Symbol::PLUS, Symbol::NT_MUL},     "<sum> -> <sum> '+' <mul>"},
        {Symbol::NT_SUM,    {Symbol::NT_SUM, Symbol::MINUS, Symbol::NT_MUL},    "<sum> -> <sum> '-' <mul>"},
        {Symbol::NT_SUM,    {Symbol::NT_MUL},                                   "<sum> -> <mul>"},
        {Symbol::NT_MUL,    {Symbol::NT_MUL, Symbol::MUL, Symbol::NT_BRAKETS},  "<mul> -> <mul> '*' <brakets>"},
        {Symbol::NT_MUL,    {Symbol::NT_MUL, Symbol::DIV, Symbol::NT_BRAKETS},  "<mul> -> <mul> '/' <brakets>"},
        {Symbol::NT_MUL,    {Symbol::NT_BRAKETS},                               "<mul> -> <brakets>"},
        {Symbol::NT_BRAKETS,{Symbol::LBRACKET, Symbol::NT_SUM, Symbol::RBRACKET}, "<brakets> -> '(' <sum> ')'"},
        {Symbol::NT_BRAKETS,{Symbol::NUMBER}, "<brakets> -> NUM"},
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
            
            for (size_t i = 0; i < body.size(); ++i) {
                if (isTerminal(body[i])) {
                    continue;
                }
                
                std::vector<Symbol> to_add;
                
                if (i == body.size() - 1) {
                    const auto& follow_head = follow_sets_[static_cast<size_t>(prod.head)];
                    to_add.insert(to_add.end(), follow_head.begin(), follow_head.end());
                }
                else {
                    Symbol next_sym = body[i + 1];
                    
                    if (isTerminal(next_sym)) {
                        to_add.push_back(next_sym);
                    }
                    else {
                        const auto& first_next = first_sets_[static_cast<size_t>(next_sym)];
                        to_add.insert(to_add.end(), first_next.begin(), first_next.end());
                    }
                }

                auto& follow_current = follow_sets_[static_cast<size_t>(body[i])];
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
            for (Symbol s : first_body_set) {
                if (first_head.insert(s).second) {
                    changed = true;
                }
            }
        }
    }
}

const std::unordered_set<Symbol>& Grammar::getFollow(Symbol non_terminal) const {
    return follow_sets_[static_cast<size_t>(non_terminal)];
}

bool Grammar::isNonTerminal(Symbol s) {
    return s >= Symbol::NT_START && s < Symbol::COUNT;
}

bool Grammar::isTerminal(Symbol s) {
    return s >= Symbol::END_OF_FILE && s < static_cast<Symbol>(slr::lexer::TokenType::COUNT);
}

Symbol Grammar::fromTokenType(lexer::TokenType token_type) {
    if (isTerminal(static_cast<Symbol>(token_type))) {
        return static_cast<Symbol>(token_type);
    }
    return Symbol::UNKNOWN;
}


#define CASE_RET_STR(type, ret) case Symbol::type: { return ret; }
std::string Grammar::symbolName(Symbol s) {
    switch(s) {
        CASE_RET_STR(END_OF_FILE, "EOF")
        CASE_RET_STR(NUMBER, "NUM")
        CASE_RET_STR(PLUS, "+")
        CASE_RET_STR(MINUS, "-")
        CASE_RET_STR(MUL, "*")
        CASE_RET_STR(DIV, "/")
        CASE_RET_STR(LBRACKET, "(")
        CASE_RET_STR(RBRACKET, ")")
        CASE_RET_STR(NT_START, "NT_START")
        CASE_RET_STR(NT_SUM, "NT_SUM")
        CASE_RET_STR(NT_MUL, "NT_MUL")
        CASE_RET_STR(NT_BRAKETS, "NT_BRAKETS")
        default: {
            return "UNKNOWN";
        }
    }
    return "UNKNOWN";
}

} // namespace slr::parser