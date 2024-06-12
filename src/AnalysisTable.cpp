#include "AnalysisTable.h"
#include <unordered_set>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

AnalysisTable::AnalysisTable(const std::string& json_file) {
    loadFromFile(json_file);
}

void AnalysisTable::loadFromFile(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    nlohmann::json json_data;
    ifs >> json_data;

    for (auto& non_terminal : json_data.items()) {
        std::string non_terminal_key = non_terminal.key();
        for (auto& terminal : non_terminal.value().items()) {
            std::string terminal_key = terminal.key();
            Production production;
            std::istringstream iss(terminal.value().get<std::string>());
            std::string symbol;
            while (iss >> symbol) {
                production.push_back(symbol);
            }
            table_[non_terminal_key][terminal_key] = production;
        }
    }
}

Production AnalysisTable::getProduction(const std::string& nonTerminal, const std::string& terminal) const {
    auto non_terminal_it = table_.find(nonTerminal);
    if (non_terminal_it != table_.end()) {
        auto terminal_it = non_terminal_it->second.find(terminal);
        if (terminal_it != non_terminal_it->second.end()) {
            return terminal_it->second;
        }
    }
    return {};
}

void AnalysisTable::printTable() const {
    for (const auto& non_terminal : table_) {
        std::cout << non_terminal.first << ":\n";
        for (const auto& terminal : non_terminal.second) {
            std::cout << "  " << terminal.first << " -> ";
            for (const auto& symbol : terminal.second) {
                std::cout << symbol << " ";
            }
            std::cout << "\n";
        }
    }
}