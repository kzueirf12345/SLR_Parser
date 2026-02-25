#pragma once

#include <limits>
#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include <set>

#include "syntaxer/Grammar.hpp"

namespace slr {
namespace syntaxer {

enum class ActionType {
    SHIFT,
    REDUCE,
    ACCEPT,
    ERROR
};

struct ParseAction {
    ActionType type;
    size_t target;      // SHIFT: state_ind, REDUCE: production_ind
    std::string rule;   // REDUCE: productions[prodution_ind].name

    constexpr inline static size_t TARGET_POISION = std::numeric_limits<size_t>::max();
    
    static ParseAction shift    (size_t state)                              noexcept;
    static ParseAction reduce   (size_t prod_ind, std::string rule_name)    noexcept;
    static ParseAction accept   ()                                          noexcept;
    static ParseAction error    ()                                          noexcept;
};


struct Item {
    size_t prod_ind; 
    size_t dot_pos;
    
    Item(size_t prod_ind, size_t dot = 0) noexcept;
    
    auto operator<=>(const Item& other) const = default;
};

using ItemSet = std::set<Item>;
using StateVec = std::vector<ItemSet>;
using StateNum = uint64_t;

class ParsingTable {

public:

    ParsingTable(const Grammar& grammar);
    
    ParseAction getAction(StateNum state, Symbol terminal) const;
    std::optional<StateNum> getGoto(StateNum state, Symbol non_terminal) const;
    
    void print(std::ostream& out = std::cout) const;
    
    size_t getStateCount() const { return states_.size(); }
    
private:

    const Grammar& grammar_;
    
    StateVec states_;
    
    std::vector<std::unordered_map<Symbol, ParseAction>> action_table_;
    std::vector<std::unordered_map<Symbol, StateNum>> goto_table_;

private:
    
    ItemSet closure(const ItemSet& items);
    ItemSet gotoState(const ItemSet& items, Symbol symbol);
    void buildCanonicalCollection();
    void buildTables();
    
    bool isCompleteItem(const Item& item) const;
    Symbol symbolAtDot(const Item& item) const;
};

}
}