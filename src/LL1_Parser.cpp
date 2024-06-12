#include "LL1_Parser.h"
#include <cstddef>
#include <iostream>

void LL1_Parser::initializeStacks() {
    input_stack_.push(Token("#", "#", 0));

    // 将tokens_反向推入输入栈
    for (auto it = tokens_.rbegin(); it != tokens_.rend(); ++it) {
        input_stack_.push(*it);
    }
    // 初始化分析栈，假设起始符号为"S"
    analysis_stack_.push("#"); // 终结符号
    analysis_stack_.push("Program"); // 起始符号
}

bool LL1_Parser::applyProduction(const std::string& nonTerminal, const std::string& terminal) {
    Production production = table_.getProduction(nonTerminal, terminal);
    std::cout << "production: " << nonTerminal << " -> ";
    for (const auto& symbol : production) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;
    if (production.empty()) {
        return false; // 无法应用产生式
    }
    // 弹出非终结符
    analysis_stack_.pop();
    // 反向推入产生式中的符号
    for (auto it = production.rbegin(); it != production.rend(); ++it) {
        if (*it != "$") { // 假设$表示空产生式
            analysis_stack_.push(*it);
        }
    }
    return true;
}

void LL1_Parser::printStacks() const {
    std::cout << "Analysis stack: ";
    std::stack<std::string> analysis_stack = analysis_stack_;
    while (!analysis_stack.empty()) {
        std::cout << analysis_stack.top() << " ";
        analysis_stack.pop();
    }
    std::cout << std::endl;

    std::cout << "Input stack: ";
    std::stack<Token> input_stack = input_stack_;
    while (!input_stack.empty()) {
        std::cout << input_stack.top().value << " ";
        input_stack.pop();
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

bool LL1_Parser::parse() {
    while (!analysis_stack_.empty() && !input_stack_.empty()) {
        std::string top = analysis_stack_.top();
        Token currentToken = input_stack_.top();

        printStacks();
        
        if (top == currentToken.value || top == "$") { // 匹配终结符或空产生式
            // std::cout << "Matched " << top << std::endl;
            analysis_stack_.pop();
            input_stack_.pop();
        }
        else if (top == "ID" && currentToken.type == "I") {
            // std::cout << "Matched ID" << std::endl;
            analysis_stack_.pop();
            input_stack_.pop();
        } 
        else if (top == "Expression" && currentToken.type == "C") {
            // std::cout << "Matched Expression" << std::endl;
            analysis_stack_.pop();
            input_stack_.pop();
        } 
        else if (isupper(top[0])) { // 非终结符
            if (!applyProduction(top, currentToken.value)) {
                std::cerr << "Failed to apply production for non-terminal " << top << " and terminal " << currentToken.value << std::endl;
                return false; // 无法应用产生式
            }
        }
        else if (top == "#" && currentToken.value == "#") {
            std::cout << "Parsing successful!" << std::endl;
            return true; // 分析成功
        } 
        else {
            return false; // 语法错误
        }
    }
    return analysis_stack_.empty() && input_stack_.empty();
}
