#include "Tokenization.h"
#include "AnalysisTable.h"
#include "LL1_Parser.h"
#include "TokenSequence.h"

int main() {
    std::string input = "int main() { return 0; }";
    Tokenization tokenizer(input, "../config/K.txt", "../config/P.txt", true);

    if (!tokenizer.Analyze()) {
        std::cerr << "Tokenization failed!" << std::endl;
        return 1;
    }

    // 打印token列表
    // tokenizer.Print_TokenList();

    TokenList tokens = tokenizer.get_final_token_list();

    AnalysisTable table("../config/AnalysisTable.json");

    // 打印分析表
    table.printTable();

    LL1_Parser parser(tokens, table);

    if (parser.parse()) {
        std::cout << "Parsing succeeded!" << std::endl;
    } else {
        std::cerr << "Parsing failed!" << std::endl;
    }

    return 0;
}
