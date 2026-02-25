#include "syntaxer/ParsingTable.hpp"

#include <iostream>
#include <queue>
#include <algorithm>
#include <cassert>

// #include "utils/concole.hpp"

namespace slr {
namespace syntaxer {

bool ParsingTable::isCompleteItem(const Item& item) const {
    const auto& prod = grammar_.getProductions()[item.prod_index];
    return item.dot_pos == prod.body.size();
}

Symbol ParsingTable::symbolAtDot(const Item& item) const {
    const auto& prod = grammar_.getProductions()[item.prod_index];
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
            for (size_t i = 0; i < grammar_.getProductionCount(); ++i) {
                const auto& prod = grammar_.getProductions()[i];
                if (prod.head == next_sym) {
                    Item new_item(i, 0);
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
            Item moved(item.prod_index, item.dot_pos + 1);
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
        
        for (int sym = 0; sym < static_cast<int>(Symbol::COUNT); ++sym) {
            Symbol symbol = static_cast<Symbol>(sym);

            if (symbol == Symbol::UNKNOWN) {
                continue;
            }

            ItemSet next = gotoState(current, symbol);

            // std::cerr << "AFTER GOTO STATE\n";
            
            if (!next.empty() && std::find(states_.begin(), states_.end(), next) == states_.end()) {
                states_.push_back(next);
                bfs_queue.push(next);
            }
        }
    }
}

void ParsingTable::buildTables() {
    int state_count = static_cast<int>(states_.size());
    action_table_.resize(state_count);
    goto_table_.resize(state_count);
    
    for (int state_ind = 0; state_ind < state_count; ++state_ind) {
        const ItemSet& items = states_[state_ind];
        
        for (const auto& item : items) {
            const auto& prod = grammar_.getProductions()[item.prod_index];
            
            if (!isCompleteItem(item)) { // SHIFT
                Symbol next_sym = symbolAtDot(item);
                
                if (Grammar::isTerminal(next_sym)) {
                    ItemSet next_state_items = gotoState(items, next_sym);
                    auto next_state_it = std::find(states_.begin(), states_.end(), next_state_items);

                    if (next_state_it != states_.end()) {
                        int target_state = static_cast<int>(next_state_it - states_.begin());
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
                            static_cast<int>(item.prod_index),
                            grammar_.productionString(item.prod_index)
                        );
                        
                    }
                }
            }
        }
        
        for (int sym = 0; sym < static_cast<int>(Symbol::COUNT); ++sym) {
            Symbol symbol = static_cast<Symbol>(sym);
            if (Grammar::isNonTerminal(symbol)) {
                ItemSet next = gotoState(items, symbol);
                if (!next.empty()) {
                    auto it = std::find(states_.begin(), states_.end(), next);
                    if (it != states_.end()) {
                        int target_state = static_cast<int>(it - states_.begin());
                        goto_table_[state_ind][symbol] = target_state;
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

ParseAction ParsingTable::getAction(int state, Symbol terminal) const {
    if (state < 0 || state >= static_cast<int>(action_table_.size())) {
        return ParseAction::error();
    }
    auto it = action_table_[state].find(terminal);
    return (it != action_table_[state].end()) ? it->second : ParseAction::error();
}

std::optional<int> ParsingTable::getGoto(int state, Symbol non_terminal) const {
    if (state < 0 || state >= static_cast<int>(goto_table_.size())) {
        return std::nullopt;
    }
    auto it = goto_table_[state].find(non_terminal);
    return (it != goto_table_[state].end()) ? std::optional<int>(it->second) : std::nullopt;
}


void ParsingTable::print() const {
    std::cout << "\n=== SLR Parsing Table ===\n";
    std::cout << "States: " << states_.size() << "\n\n";
    
    std::cout << "ACTION table:\n";
    std::cout << "State\t";

    for (int s = 0; s < static_cast<int>(Symbol::COUNT); ++s) {
        Symbol sym = static_cast<Symbol>(s);

        if (Grammar::isTerminal(sym)) {
            std::cout << Grammar::symbolName(sym) << "\t";
        }
    }
    std::cout << "\n";
    
    for (int state = 0; state < static_cast<int>(states_.size()); ++state) {
        std::cout << state << "\t";

        for (int s = 0; s < static_cast<int>(Symbol::COUNT); ++s) {

            Symbol sym = static_cast<Symbol>(s);

            if (Grammar::isTerminal(sym)) {

                auto it = action_table_[state].find(sym);
                if (it != action_table_[state].end()) {
                    const auto& act = it->second;

                    switch (act.type) {
                        case ActionType::SHIFT:
                            std::cout << "s" << act.target << "\t"; break;
                        case ActionType::REDUCE:
                            std::cout << "r" << act.target << "\t"; break;
                        case ActionType::ACCEPT:
                            std::cout << "acc\t"; break;
                        case ActionType::ERROR:
                            std::cout << "err\t"; break;
                    }
                } 
                else {
                    std::cout << "-\t";
                }
            }
        }
        std::cout << "\n";
    }
    
    std::cout << "\nGOTO table:\n";
    std::cout << "State\t";
    for (int s = 0; s < static_cast<int>(Symbol::COUNT); ++s) {
        Symbol sym = static_cast<Symbol>(s);
        if (Grammar::isNonTerminal(sym)) {
            std::cout << Grammar::symbolName(sym) << "\t";
        }
    }
    std::cout << "\n";
    
    for (int state = 0; state < static_cast<int>(states_.size()); ++state) {
        std::cout << state << "\t";

        for (int s = 0; s < static_cast<int>(Symbol::COUNT); ++s) {
            Symbol sym = static_cast<Symbol>(s);

            if (Grammar::isNonTerminal(sym)) {
                auto it = goto_table_[state].find(sym);

                if (it != goto_table_[state].end()) {
                    std::cout << it->second << "\t";
                } 
                else {
                    std::cout << "-\t";
                }
            }
        }
        std::cout << "\n";
    }
}

}
}