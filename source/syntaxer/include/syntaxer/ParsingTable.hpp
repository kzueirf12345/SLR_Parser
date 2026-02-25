#pragma once

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
    int target;         // SHIFT: state_ind, REDUCE: production_ind
    std::string rule;   // REDUCE: productions[prodution_ind].name
    
    static ParseAction shift(int state) {
        return {ActionType::SHIFT, state, ""};
    }
    static ParseAction reduce(int prod_idx, std::string rule_name) {
        return {ActionType::REDUCE, prod_idx, std::move(rule_name)};
    }
    static ParseAction accept() {
        return {ActionType::ACCEPT, -1, ""};
    }
    static ParseAction error() {
        return {ActionType::ERROR, -1, ""};
    }
};


struct Item {
    size_t prod_index; 
    size_t dot_pos;
    
    Item(size_t prod, size_t dot = 0) : prod_index(prod), dot_pos(dot) {}
    
    auto operator<=>(const Item& other) const = default;
};


using ItemSet = std::set<Item>;
using StateVec = std::vector<ItemSet>;

class ParsingTable {

public:

    ParsingTable(const Grammar& grammar);
    
    ParseAction getAction(int state, Symbol terminal) const;
    std::optional<int> getGoto(int state, Symbol non_terminal) const;
    
    void print() const;
    
    int getStateCount() const { return static_cast<int>(states_.size()); }
    
private:

    const Grammar& grammar_;
    
    StateVec states_;
    
    std::vector<std::unordered_map<Symbol, ParseAction>> action_table_;
    std::vector<std::unordered_map<Symbol, int>> goto_table_;
    
    ItemSet closure(const ItemSet& items);
    ItemSet gotoState(const ItemSet& items, Symbol symbol);
    void buildCanonicalCollection();
    void buildTables();
    
    bool isCompleteItem(const Item& item) const;
    Symbol symbolAtDot(const Item& item) const;
};

}
}