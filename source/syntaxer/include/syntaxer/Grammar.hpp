#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "lexer/tokens.hpp"

namespace slr {
namespace syntaxer {

enum class Symbol : int {
    // TokenType + offset
    UNKNOWN = 0,
    END_OF_FILE,
    NUMBER,
    ID,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LBRACKET,
    RBRACKET,
    
    // Нетерминалы
    NT_START,
    NT_SUM,
    NT_MUL,
    NT_BRAKETS,
    
    COUNT
};

struct Production {
    Symbol head;
    std::vector<Symbol> body;
    std::string name;
};

class Grammar {

public:
    Grammar();
    
    const std::vector<Production>&  getProductions()                    const noexcept;
    size_t                          getProductionCount()                const noexcept;
    std::string                     productionString(size_t prod_ind)   const noexcept;
    
    const std::unordered_set<Symbol>& getFollow(Symbol non_terminal) const;
    const std::unordered_set<Symbol>& getFirst(Symbol symbol) const;

    void print(std::ostream& out = std::cout) const;
    
    static bool isNonTerminal(Symbol s);
    static bool isTerminal(Symbol s);
    
    static Symbol fromTokenType(lexer::TokenType token_type);
    
    static std::string getSymbolStr         (Symbol s);
    static std::string getPrettySymbolStr   (Symbol s);

private:
    std::vector<Production> productions_;
    std::vector<std::unordered_set<Symbol>> first_sets_;
    std::vector<std::unordered_set<Symbol>> follow_sets_;
    
    void buildFirstSets();
    void buildFollowSets();
};


}
}