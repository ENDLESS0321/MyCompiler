#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>

using Production = std::vector<std::string>;
using Productions = std::unordered_map<std::string, std::vector<Production>>;

class Grammar {
public:
    // 从文件中加载文法规则
    Grammar(const std::string& filename);
    const Productions& getProductions() const;
    void printProductions();

private:
    Productions productions_;

    void loadFromFile(const std::string& filename);
};

#endif // GRAMMAR_H
