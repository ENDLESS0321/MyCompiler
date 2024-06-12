#ifndef LL1_PARSER_H
#define LL1_PARSER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
#include "AnalysisTable.h"
#include "Tokenization.h"

class LL1_Parser {
private:
    TokenList tokens_;
    AnalysisTable table_;
    std::stack<std::string> analysis_stack_; // 分析栈
    std::stack<Token> input_stack_; // 输入栈

    void initializeStacks();
    bool applyProduction(const std::string& nonTerminal, const std::string& terminal);
    void printStacks() const;
public:
    LL1_Parser(const TokenList& tokens, const AnalysisTable& table)
        : tokens_(tokens), table_(table) {
        initializeStacks();
    }

    bool parse();
};

#endif // LL1_PARSER_H
