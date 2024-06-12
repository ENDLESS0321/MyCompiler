#ifndef TOKENIZATION_H
#define TOKENIZATION_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>

struct Token {
    std::string type;
    std::string value;
    int id;
    Token(std::string type, std::string value, int id) : type(type), value(value), id(id) {}
};

typedef std::vector<Token> TokenList;


class Tokenization {
private: 
    enum State {
        Init,
        Reading_string,
        Reading_digit,
        Reading_operator,
        Error
    };

    std::unordered_map<std::string, int> keyword_list_;                     // 关键字表
    std::unordered_map<std::string, int> delimiter_table_;                  // 界符表
    std::unordered_map<std::string, int> symbol_table_;                     // 符号表, 为用户自定义的标识符，代码为1
    std::unordered_map<std::string, int> constant_table_;                   // 常数表

    TokenList final_token_list_;                                            // 最终的token列表
    
    std::stack<char> brackets_stack_;           // 括号栈
    std::vector<char> analyze_stack_;           // 分析栈
    std::stack<char> operator_stack_;           // 操作符栈

    std::string input_;
    State current_state_;

    std::string keyword_list_file_;
    std::string delimiter_table_file_;

    bool load_parameter_from_file_ = false;

public:
    Tokenization(std::string input, std::string keyword_list_file, std::string delimiter_table_file, bool load_parameter_from_file = false);

    bool Analyze();
    
    bool is_digit(char c) {
        return std::isdigit(c);
    }
    
    bool is_char(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    bool is_space(char c) {
        return c == ' ';
    }

    bool is_delimiter(char c) {
        std::string cc;
        cc += c;
        auto it = delimiter_table_.find(cc);
        if (it != delimiter_table_.end()) {
            return true;
        }
        return false;
    }

    int get_delimiter_index(char c) {
        std::string cc;
        cc += c;
        auto it = delimiter_table_.find(cc);
        if (it != delimiter_table_.end()) {
            return it->second;
        }
        return -1;
    }

    bool Analyze_current_token () {
        std::string current_token;
        for (const auto c : analyze_stack_) {
            current_token += c;
        }
        analyze_stack_.clear();

        auto it_1 = keyword_list_.find(current_token);
        if (it_1 != keyword_list_.end()) {
            final_token_list_.push_back(Token("K", current_token, keyword_list_[current_token]));
            return true;
        }

        bool is_symbol = false;
        auto it_2 = symbol_table_.find(current_token);
        if (it_2 != symbol_table_.end()) {
            final_token_list_.push_back(Token("I", current_token, symbol_table_[current_token]));
            is_symbol = true;
            return true;    
        }

        if (!is_symbol) {
            int number = symbol_table_.size() + 1;
            symbol_table_.insert(std::make_pair(current_token, number));
            final_token_list_.push_back(Token("I", current_token, number));
        }
        return true;
    }

    bool Analyze_current_digit() {
        std::string current_token;
        for (const auto c : analyze_stack_) {
            current_token += c;
        }
        analyze_stack_.clear();
        bool is_constant = false;
        auto it = constant_table_.find(current_token);
        if (it != constant_table_.end()) {
            final_token_list_.push_back(Token("C", current_token, constant_table_[current_token]));
        }
        else {
            constant_table_.insert(std::make_pair(current_token, constant_table_.size() + 1));
            final_token_list_.push_back(Token("C", current_token, constant_table_.size()));
        }
        return true;
    }

    void Print_DelimiterTable() {
        for (const auto delimiter : delimiter_table_) {
            std::cout << "Type: " << delimiter.first << ", Value: " << delimiter.second << std::endl;
        }
    }

    void Print_KeywordList() {
        for (const auto keyword : keyword_list_) {
            std::cout << "Type: " << keyword.first << ", Value: " << keyword.second << std::endl;
        }
    }

    void Print_SymbolTable() {
        for (const auto symbol : symbol_table_) {
            std::cout << "Type: " << symbol.first << ", Value: " << symbol.second << std::endl;
        }
    }

    void Print_ConstantTable() {
        for (const auto constant : constant_table_) {
            std::cout << "Type: " << constant.first << ", Value: " << constant.second << std::endl;
        }
    }

    void Print_TokenList() {
        for (const auto token : final_token_list_) {
            std::cout << "Type: " << token.type << ", Value: " << token.value << ", ID: " << token.id << std::endl;
        }
    }

    void Print_for_Test() {
        std::cout << "Token :";
        for (const auto token : final_token_list_) {
            std::cout << "(" << token.type << " " << token.id << ")";
        }
        std::cout << std::endl;

        std::cout << "I :";
        for (size_t i = 0; i < symbol_table_.size(); i++) {
            for (const auto symbol : symbol_table_) {
                if (symbol.second == i + 1) {
                    std::cout << symbol.first << " ";
                }
            }
        }
        std::cout << std::endl;

        std::cout << "C :";
        for (size_t i = 0; i < constant_table_.size(); i++) {
            for (const auto constant : constant_table_) {
                if (constant.second == i + 1) {
                    std::cout << constant.first << " ";
                }
            }
        }
        std::cout << std::endl;
    }

    TokenList get_final_token_list() {
        return final_token_list_;
    }
};


#endif // TOKENIZATION_H