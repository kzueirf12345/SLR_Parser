#include "syntaxer/ParsingTable.hpp"

#include <iomanip>
#include <iostream>
#include <queue>
#include <algorithm>
#include <cassert>

// #include "utils/concole.hpp"

namespace slr {
namespace syntaxer {

ParseAction ParseAction::shift(size_t state) noexcept {
    return {ActionType::SHIFT, state, ""};
}
ParseAction ParseAction::reduce(size_t prod_ind, std::string rule_name) noexcept {
    return {ActionType::REDUCE, prod_ind, std::move(rule_name)};
}
ParseAction ParseAction::accept() noexcept {
    return {ActionType::ACCEPT, TARGET_POISION, ""};
}
ParseAction ParseAction::error() noexcept {
    return {ActionType::ERROR, TARGET_POISION, ""};
}

Item::Item(size_t prod_ind, size_t dot) noexcept 
    :   prod_ind(prod_ind)
    ,   dot_pos(dot) 
{}

bool ParsingTable::isCompleteItem(const Item& item) const {
    const auto& prod = grammar_.getProductions()[item.prod_ind];
    return item.dot_pos == prod.body.size();
}

Symbol ParsingTable::symbolAtDot(const Item& item) const {
    const auto& prod = grammar_.getProductions()[item.prod_ind];
    if (item.dot_pos >= prod.body.size()) {
        return Symbol::UNKNOWN;
    }
    return prod.body[item.dot_pos];
}

ItemSet ParsingTable::closure(const ItemSet& items) {
    ItemSet result = items;
    std::vector<Item> stack(items.begin(), items.end());
    
    while (!stack.empty()) {
        Item current = stack.back();
        stack.pop_back();
        
        // if (current.dot_pos < grammar_.getProductions()[current.prod_index].body.size())
        // std::cerr << RED_FORMAT << current.dot_pos << " " << grammar_.getProductions()[current.prod_index].name << " " << grammar_.getProductions()[current.prod_index].body.size() << NORMAL_FORMAT << std::endl;
        Symbol next_sym = symbolAtDot(current);

        if (Grammar::isNonTerminal(next_sym)) {
            for (size_t prod_ind = 0; prod_ind < grammar_.getProductionCount(); ++prod_ind) {
                const auto& prod = grammar_.getProductions()[prod_ind];

                if (prod.head == next_sym) {
                    Item new_item(prod_ind, 0);

                    if (result.insert(new_item).second) {
                        stack.push_back(new_item);
                    }
                }
            }
        }
    }
    
    return result;
}

ItemSet ParsingTable::gotoState(const ItemSet& items, Symbol symbol) {
    ItemSet result;
    
    for (const auto& item : items) {
        if (symbolAtDot(item) == symbol) {
            Item moved(item.prod_ind, item.dot_pos + 1);
            result.insert(std::move(moved));
        }
    }
    return closure(result);
}

void ParsingTable::buildCanonicalCollection() {
    states_.clear();
    std::queue<ItemSet> bfs_queue;
    
    ItemSet initial;
    for (size_t i = 0; i < grammar_.getProductionCount(); ++i) {
        const auto& prod = grammar_.getProductions()[i];
        if (prod.head == Symbol::NT_START) {
            initial.insert(Item(i, 0));
            break;
        }
    }
    initial = closure(initial);
    
    states_.push_back(initial);
    bfs_queue.push(initial);
    
    while (!bfs_queue.empty()) {
        ItemSet current = bfs_queue.front();
        bfs_queue.pop();
        
        for (int i = 1; i < static_cast<int>(Symbol::COUNT); ++i) {
            Symbol sym = static_cast<Symbol>(i);

            ItemSet next = gotoState(current, sym);

            // std::cerr << "AFTER GOTO STATE\n";
            
            if (!next.empty() && std::find(states_.begin(), states_.end(), next) == states_.end()) {
                states_.push_back(next);
                bfs_queue.push(next);
            }
        }
    }
}

void ParsingTable::buildTables() {
    size_t state_count = states_.size();
    action_table_.resize(state_count);
    goto_table_.resize(state_count);
    
    for (size_t state_ind = 0; state_ind < state_count; ++state_ind) {
        const ItemSet& items = states_[state_ind];
        
        for (const auto& item : items) {
            const auto& prod = grammar_.getProductions()[item.prod_ind];
            
            if (!isCompleteItem(item)) { // SHIFT
                Symbol next_sym = symbolAtDot(item);
                
                if (Grammar::isTerminal(next_sym)) {
                    ItemSet next_state_items = gotoState(items, next_sym);
                    auto next_state_it = std::find(states_.begin(), states_.end(), next_state_items);

                    if (next_state_it != states_.end()) {
                        StateNum target_state = static_cast<StateNum>(next_state_it - states_.begin());
                        action_table_[state_ind][next_sym] = ParseAction::shift(target_state);
                    }
                }
            }
            else { //REDUCE
                if (prod.head == Symbol::NT_START && item.dot_pos == 1) {
                    action_table_[state_ind][Symbol::END_OF_FILE] = ParseAction::accept();
                } 
                else {
                    const auto& follow = grammar_.getFollow(prod.head);
                    for (Symbol term : follow) {

                        assert(action_table_[state_ind].find(term) == action_table_[state_ind].end());
                        
                        action_table_[state_ind][term] = ParseAction::reduce(
                            item.prod_ind,
                            grammar_.productionString(item.prod_ind)
                        );
                        
                    }
                }
            }
        }
        
        for (int i = 0; i < static_cast<int>(Symbol::COUNT); ++i) {
            Symbol sym = static_cast<Symbol>(i);

            if (Grammar::isNonTerminal(sym)) {
                ItemSet next = gotoState(items, sym);

                if (!next.empty()) {
                    auto it = std::find(states_.begin(), states_.end(), next);
                    if (it != states_.end()) {
                        StateNum target_state = static_cast<StateNum>(it - states_.begin());
                        goto_table_[state_ind][sym] = target_state;
                    }
                }
            }
        }
    }
}

ParsingTable::ParsingTable(const Grammar& grammar) 
    : grammar_(grammar) 
{
    buildCanonicalCollection();
    buildTables();
}

ParseAction ParsingTable::getAction(StateNum state, Symbol terminal) const {
    if (state >= static_cast<StateNum>(action_table_.size())) {
        return ParseAction::error();
    }
    auto it = action_table_[state].find(terminal);
    return (it != action_table_[state].end()) ? it->second : ParseAction::error();
}

std::optional<StateNum> ParsingTable::getGoto(StateNum state, Symbol non_terminal) const {
    if (state >= static_cast<StateNum>(goto_table_.size())) {
        return std::nullopt;
    }
    auto it = goto_table_[state].find(non_terminal);
    return (it != goto_table_[state].end()) ? std::optional<StateNum>(it->second) : std::nullopt;
}


void ParsingTable::print(std::ostream& out) const {
    
    out << "\n=== SLR Parsing Table ===\n";
    out << "States: " << states_.size() << "\n";
    

    out << "\n===ACTION table===\n";
    
    out << std::right << std::setw(15) << "State";

    for (int s = 0; s < static_cast<int>(Symbol::COUNT); ++s) {
        Symbol sym = static_cast<Symbol>(s);
        if (Grammar::isTerminal(sym)) {
            out << std::setw(15) << Grammar::getSymbolStr(sym);
        }
    }
    
    out << "\n" << std::string(100, '-') << "\n";
    
    for (StateNum state = 0; state < static_cast<StateNum>(states_.size()); ++state) {
        out << std::right << std::setw(15) << state;
        
        for (int i = 0; i < static_cast<int>(Symbol::COUNT); ++i) {
            Symbol sym = static_cast<Symbol>(i);
            
            if (Grammar::isTerminal(sym)) {
                auto it = action_table_[state].find(sym);

                if (it != action_table_[state].end()) {
                    const auto& act = it->second;
                    std::string cell;

                    switch (act.type) {
                        case ActionType::SHIFT:
                            cell = "s" + std::to_string(act.target); 
                            break;
                        case ActionType::REDUCE:
                            cell = "r" + std::to_string(act.target); 
                            break;
                        case ActionType::ACCEPT:
                            cell = "acc"; 
                            break;
                        case ActionType::ERROR:
                            cell = "err"; 
                            break;
                    }
                    out << std::setw(15) << cell;
                } 
                else {
                    out << std::setw(15) << "-";
                }
            }
        }
        out << "\n";
    }
    
    out << "\n===GOTO table===\n";
    
    out << std::right << std::setw(15) << "State";

    for (int i = 0; i < static_cast<int>(Symbol::COUNT); ++i) {
        Symbol sym = static_cast<Symbol>(i);
        if (Grammar::isNonTerminal(sym)) {
            out << std::setw(15) << Grammar::getSymbolStr(sym);
        }
    }

    out << "\n" << std::string(100, '-') << "\n";
    
    for (StateNum state_ind = 0; state_ind < static_cast<StateNum>(states_.size()); ++state_ind) {
        out << std::right << std::setw(15) << state_ind;
        
        for (int i = 0; i < static_cast<int>(Symbol::COUNT); ++i) {
            Symbol sym = static_cast<Symbol>(i);

            if (Grammar::isNonTerminal(sym)) {
                auto it = goto_table_[state_ind].find(sym);
                if (it != goto_table_[state_ind].end()) {
                    out << std::setw(15) << it->second;
                } 
                else {
                    out << std::setw(15) << "-";
                }
            }
        }
        out << "\n";
    }
}

}
}