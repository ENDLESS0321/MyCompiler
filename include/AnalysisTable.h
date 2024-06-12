#ifndef ANALYSISTABLE_H
#define ANALYSISTABLE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp> // 使用nlohmann/json库进行JSON解析

typedef std::vector<std::string> Production;
typedef std::unordered_map<std::string, std::unordered_map<std::string, Production>> Table;

class AnalysisTable {
private:
    Table table_;
public:
    AnalysisTable(const std::string& json_file);
    void loadFromFile(const std::string& filename);
    Production getProduction(const std::string& nonTerminal, const std::string& terminal) const;
    void printTable() const;
};

#endif // ANALYSISTABLE_H
