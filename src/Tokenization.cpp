#include "Tokenization.h"
#include <iostream>

Tokenization::Tokenization(std::string input, std::string keyword_list_file, std::string delimiter_table_file, bool load_parameter_from_file) {
    input_ = input;
    current_state_ = Init;
    load_parameter_from_file_ = load_parameter_from_file;

    if (load_parameter_from_file_) {
        // 读取关键字表和界符表，从文件中读取
        keyword_list_file_ = keyword_list_file;
        delimiter_table_file_ = delimiter_table_file;

        std::ifstream keyword_file(keyword_list_file_);
        std::ifstream delimiter_file(delimiter_table_file_);

        std::string line;
        while (std::getline(keyword_file, line)) {
            keyword_list_.insert(std::make_pair(line, keyword_list_.size() + 1));
        }

        while (std::getline(delimiter_file, line)) {
            delimiter_table_.insert(std::make_pair(line, delimiter_table_.size() + 1));
        }
    }
    else {
        keyword_list_.insert(std::make_pair("int", 1));
        keyword_list_.insert(std::make_pair("void", 2));
        keyword_list_.insert(std::make_pair("break", 3));
        keyword_list_.insert(std::make_pair("float", 4));
        keyword_list_.insert(std::make_pair("while", 5));
        keyword_list_.insert(std::make_pair("do", 6));
        keyword_list_.insert(std::make_pair("struct", 7));
        keyword_list_.insert(std::make_pair("const", 8));
        keyword_list_.insert(std::make_pair("case", 9));
        keyword_list_.insert(std::make_pair("for", 10));
        keyword_list_.insert(std::make_pair("return", 11));
        keyword_list_.insert(std::make_pair("if", 12));
        keyword_list_.insert(std::make_pair("default", 13));
        keyword_list_.insert(std::make_pair("else", 14));

        delimiter_table_.insert(std::make_pair("-", 1));
        delimiter_table_.insert(std::make_pair("/", 2));
        delimiter_table_.insert(std::make_pair("(", 3));
        delimiter_table_.insert(std::make_pair(")", 4));
        delimiter_table_.insert(std::make_pair("==", 5));
        delimiter_table_.insert(std::make_pair("<=", 6));
        delimiter_table_.insert(std::make_pair("<", 7));
        delimiter_table_.insert(std::make_pair("+", 8));
        delimiter_table_.insert(std::make_pair("*", 9));
        delimiter_table_.insert(std::make_pair(">", 10));
        delimiter_table_.insert(std::make_pair("=", 11));
        delimiter_table_.insert(std::make_pair(",", 12));
        delimiter_table_.insert(std::make_pair(";", 13));
        delimiter_table_.insert(std::make_pair("++", 14));
        delimiter_table_.insert(std::make_pair("{", 15));
        delimiter_table_.insert(std::make_pair("}", 16));
    }
}

// 整体分析思路：
// 1. 按行读入，逐个字符进行分析
// 2. 读入一个字符，先判断是否为字母，如果是字母，进入Reading_string状态
//    string有可能是关键字，也有可能是标识符
// 3. 如果是数字，进入Reading_digit状态
// 4. 如果是空格，进入Init状态，即一个token结束，重新开始
// 5. 如果是界符，进入Init状态，即一个token结束，重新开始
bool Tokenization::Analyze() {
    for (const char c : input_) {
        switch (current_state_) {
        case Init:
            if (is_char(c)) {
                // std::cout << "Reading string" << std::endl;
                // std::cout << c << std::endl;
                current_state_ = Reading_string;
                analyze_stack_.push_back(c);
            }
            else if (is_digit(c)) {
                current_state_ = Reading_digit;
                analyze_stack_.push_back(c);
            }
            else if (is_delimiter(c)) {
                if (!analyze_stack_.empty()) {
                    if (current_state_ == Reading_digit) {
                        Analyze_current_digit();
                    } else {
                        Analyze_current_token();
                    }
                }
                final_token_list_.push_back(Token("P", std::string(1, c), get_delimiter_index(c)));
                current_state_ = Init;
            }
            else if (is_space(c)) {
                current_state_ = Init;
            }
            else
                current_state_ = Error;
            break;

        case Reading_string:
            if (is_char(c) || is_digit(c)) {
                current_state_ = Reading_string;
                analyze_stack_.push_back(c);
            }
            else if (is_space(c)) {
                Analyze_current_token();
                current_state_ = Init;
            }
            else if (is_delimiter(c)) {
                Analyze_current_token();
                final_token_list_.push_back(Token("P", std::string(1, c), get_delimiter_index(c)));
                current_state_ = Init;
            }
            else {
                current_state_ = Error;
            }
            break;

        case Reading_digit:
            if (is_digit(c)) {
                current_state_ = Reading_digit;
                analyze_stack_.push_back(c);
            }
            else if (is_delimiter(c)) {
                Analyze_current_digit();
                final_token_list_.push_back(Token("P", std::string(1, c), get_delimiter_index(c)));
                current_state_ = Init;
            }
            else if (is_space(c)) {
                Analyze_current_digit();
                current_state_ = Init;
            }
            else {
                current_state_ = Error;
            }
            break;

        case Reading_operator:
            if (is_delimiter(c)) {
                current_state_ = Reading_operator;
                analyze_stack_.push_back(c);
            }
            else if (is_space(c)) {
                current_state_ = Init;
            }
            else {
                current_state_ = Error;
            }
            break;

        case Error:
            break;
        }
    }

    // 处理剩余的分析栈中的内容
    if (!analyze_stack_.empty()) {
        if (current_state_ == Reading_digit) {
            Analyze_current_digit();
        } else {
            Analyze_current_token();
        }
    }

    if (current_state_ == Error) {
        return false;
    } else {
        return true;
    }
}