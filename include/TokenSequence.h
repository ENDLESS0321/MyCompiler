#ifndef TOKENSEQUENCE_H
#define TOKENSEQUENCE_H

#include "includes.h"
#include "Tokenization.h"

class TokenSequence
{
public:
    // TokenSequence();
    void scan(std::string filepath);
    void printAll();
    void printToken();
    void initKeyWord();
    void initDelimiters();
    int get_kind(char ch);
    void test();

    TokenList getToken_list(std::string filepath)
    {
        initKeyWord();
        initDelimiters();

        scan(filepath);
        return Token_list;
    }

private:
    std::unordered_map<std::string, int> KeyWord;    // �ؼ��ֱ���K
    std::unordered_map<std::string, int> Punctuator; // �����,P
    std::unordered_map<std::string, int> Constant;   // ������,����������С�� ,Con
    std::unordered_map<std::string, int> Identifier; // ��ʶ������,I
    std::unordered_map<std::string, int> Character;  // �ַ�����,Ch
    std::unordered_map<std::string, int> String;     // �ַ�������,St
    std::unordered_map<std::string, int> Array;      // ����� Arr
    std::unordered_map<std::string, int> Error;      // ������
    TokenList Token_list;                  // token���б�
    int IdKey = 1;
    int IdPunctuator = 1;
    int IdConstant = 1;
    int IdToken = 1;
    int IdIdentifier = 1;
    int IdCharacter = 1;
    int IdString = 1;
    int IdArray = 1;
    int IdError = 1;
};


#endif // TOKENSEQUENCE_H