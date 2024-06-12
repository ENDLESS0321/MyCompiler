#include "Grammar.h"
#include <iostream>
Grammar::Grammar(const std::string& filename) {
    loadFromFile(filename);
}

const Productions& Grammar::getProductions() const {
    return productions_;
}

void Grammar::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string lhs;
        if (std::getline(iss, lhs, '-') && iss.get() == '>') {
            std::string rhs;
            std::getline(iss, rhs);
            Production production;
            std::istringstream rhsStream(rhs);
            std::string symbol;
            while (rhsStream >> symbol) {
                production.push_back(symbol);
            }
            productions_[lhs].push_back(production);
        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }
}

void Grammar::printProductions() {
    for (const auto& [lhs, rhs] : productions_) {
        for (const auto& production : rhs) {
            std::cout << lhs << " -> ";
            for (const auto& symbol : production) {
                std::cout << symbol << " ";
            }
            std::cout << std::endl;
        }
    }
}