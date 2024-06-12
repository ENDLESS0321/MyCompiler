#include "TokenSequence.h"
void TokenSequence::scan(std::string filepath)
{
    std::ifstream infile;
    infile.open(filepath, std::ios::in);
    if (!infile.is_open())
    {
        std::cout << "Cin�ļ���ȡʧ��" << "\n";
        return;
    }
    // test();
    std::string buf;
    while (getline(infile, buf)) // 1Ϊ���֣�2Ϊ��ĸ��3Ϊ�������4Ϊ�ָ�����5Ϊ����
    {
        for (int i = 0; i < buf.size(); i++)
        {
            if (buf[i] == ' ')
                continue;
            else if (get_kind(buf[i]) == 2 || buf[i] == '_')
            { // ���ַ�Ϊ��ĸ��Ϊ��ʶ�����߹ؼ���
                std::string tmp;
                bool is_Identifier = false;
                bool is_Aarry = false;                             // �ж��Ƿ�Ϊ��ʶ�����Ƿ�Ϊ����
                int leftNum = 0, rightNum = 0, lPos = 0, rPos = 0; // ��¼[,]���������ʹ洢ѭ����ʼλ��
                while (get_kind(buf[i]) == 2 || get_kind(buf[i]) == 1 || buf[i] == '_' || buf[i] == '[' || buf[i] == ']')
                {
                    if (buf[i] == '[')
                        leftNum++, lPos = i;
                    if (buf[i] == ']')
                        rightNum++, rPos = i;
                    if (get_kind(buf[i]) == 1)
                        is_Identifier = true; // �������ֳ���
                    tmp += buf[i];
                    i++;
                    if (i >= buf.size())
                        break;
                }
                i--;
                if (leftNum || rightNum)
                {                          // �����[,����]���֣���Ϊ����
                    bool is_Error = false; // �ж��Ƿ�Ϊ��������
                    std::string tmpNum;
                    for (int j = lPos + 1; j < rPos; j++)
                    {
                        tmpNum += buf[j]; // �����[]֮����ַ��������һ�������ֵı�ʶ��Ҳ�ǶԵģ�����������ж�
                        if (get_kind(buf[j]) != 1)
                        { //[��]֮����ַ�ֻ��Ϊ����
                            is_Error = true;
                        }
                    }

                    if (leftNum != 1 || rightNum != 1)
                        is_Error = true; //[��]��ֻ�ܳ���һ��
                    if (is_Error)
                    {
                        if (Error.count(tmp) == 0)
                        { // ���Error���Ѿ�����
                            Error[tmp] = IdError++;
                        }
                        Token_list.push_back({"Error", tmp, Error[tmp]}); // �Ž�Token��
                    }
                    else
                    {
                        if (Array.count(tmp) == 0)
                        { // û����Ļ��Ž�Array��
                            Array[tmp] = IdArray++;
                        }
                        Token_list.push_back({"Arr", tmp, Array[tmp]});
                    }
                }
                else if (is_Identifier)
                { // �������飬���������֣��Ǿ��Ǳ�ʶ��
                    if (Identifier.count(tmp) == 0)
                    {
                        Identifier[tmp] = IdIdentifier++;
                    }
                    Token_list.push_back({"I", tmp, Identifier[tmp]});
                }
                else
                {
                    if (KeyWord.count(tmp))
                    {
                        Token_list.push_back({"K", tmp, KeyWord[tmp]});
                    }
                    else
                    { // �ؼ��ֱ��Ҳ����Ǿ��Ǳ�ʶ��
                        if (Identifier.count(tmp) == 0)
                        {
                            Identifier[tmp] = IdIdentifier++;
                        }
                        Token_list.push_back({"I", tmp, Identifier[tmp]});
                    }
                }
            }

            else if (get_kind(buf[i]) == 1)
            {                // ��������
                int num = 0; // С���������
                bool is_Error = false;
                std::string tmp;
                // ��û����;���������
                while (buf[i] != ';' && get_kind(buf[i]) != 3)
                { // int a=123a; Error
                    if (buf[i] == '.')
                        num++;
                    if (get_kind(buf[i]) != 1 && buf[i] != '.')
                        is_Error = true;
                    tmp += buf[i];
                    i++;
                    if (i >= buf.size())
                        break;
                }
                i--;
                if (num > 1)
                    is_Error = true;
                if (is_Error)
                {
                    if (Error.count(tmp) == 0)
                    {
                        Error[tmp] = IdError++;
                    }
                    Token_list.push_back({"Error", tmp, Error[tmp]});
                }
                else
                {
                    if (Constant.count(tmp) == 0)
                    {
                        Constant[tmp] = IdConstant++;
                    }
                    Token_list.push_back({"Con", tmp, Constant[tmp]});
                }
            }

            else if (buf[i] == '\'')
            {                // �����ַ�����
                int num = 0; // ��'������
                std::string tmp;
                while (buf[i] != ';')
                {
                    if (buf[i] == '\'')
                        num++;
                    tmp += buf[i];
                    i++;
                    if (i >= buf.size())
                        break;
                }
                i--;
                bool is_Error = false;
                if (num != 2)
                    is_Error = true;
                if ((int)tmp.size() - 2 != 1)
                    is_Error = true;
                if (is_Error)
                {
                    if (Error.count(tmp) == 0)
                    {
                        Error[tmp] = IdError++;
                    }
                    Token_list.push_back({"Error", tmp, Error[tmp]});
                }
                else
                {
                    if (Character.count(tmp) == 0)
                    {
                        Character[tmp] = IdCharacter++;
                    }
                    Token_list.push_back({"Ch", tmp, Character[tmp]});
                }
            }

            else if (buf[i] == '"')
            {                // std::string a=  "wag  a aw&&91 12"a  ;
                int num = 0; // ��"������
                int Pos = 0; // �����һ��"���ֵ�λ�ã�Ӧ��������� std::string a="abcd"ab; ����Ҳ��Error
                std::string tmp;
                while (buf[i] != ';')
                {
                    if (buf[i] == '"')
                        num++, Pos = i;
                    tmp += buf[i];
                    i++;
                    if (i >= buf.size())
                        break;
                }
                i--;
                bool is_Error = false;
                if (num != 2)
                    is_Error = true;
                for (int j = Pos + 1; j <= i; j++)
                {
                    if (buf[j] != ' ')
                        is_Error = true;
                }
                if (is_Error)
                {
                    if (Error.count(tmp) == 0)
                    {
                        Error[tmp] = IdError++;
                    }
                    Token_list.push_back({"Error", tmp, Error[tmp]});
                }
                else
                {
                    if (String.count(tmp) == 0)
                    {
                        String[tmp] = IdString++;
                    }
                    Token_list.push_back({"St", tmp, String[tmp]});
                }
            }

            else if (get_kind(buf[i]) == 3 || get_kind(buf[i]) == 4)
            { // ������ͨ���
                std::string tmp;
                tmp += buf[i];
                if (buf[i] == '&' && buf[i + 1] == '&' || buf[i] == '|' && buf[i + 1] == '|' || buf[i] == '%' && buf[i + 1] == '=' || buf[i] == '>' && buf[i + 1] == '=' || buf[i] == '<' && buf[i + 1] == '=' || buf[i] == '=' && buf[i + 1] == '=' || buf[i] == '+' && buf[i + 1] == '+' || buf[i] == '-' && buf[i + 1] == '-' || buf[i] == '>' && buf[i + 1] == '>' || buf[i] == '<' && buf[i + 1] == '<')
                {
                    tmp += buf[i + 1];
                    i++;
                }
                Token_list.push_back({"P", tmp, Punctuator[tmp]});
            }
            else
            {
                std::string tmp;
                tmp += buf[i];
                if (Error.count(tmp) == 0)
                {
                    Error[tmp] = IdError++;
                }
                Token_list.push_back({"Error", tmp, Error[tmp]});
            }
        }
    }
}

void TokenSequence::printAll()
{

    std::cout << std::setw(58) << "KeyWord\n";
    std::cout << std::setw(35) << "value" << std::setw(36) << "id\n\n";
    for (auto i : KeyWord)
        std::cout << std::setw(35) << i.first << std::setw(35) << i.second << "\n";

    std::cout << std::setw(58) << "Delimiters\n";
    std::cout << std::setw(35) << "value" << std::setw(36) << "id\n\n";
    for (auto i : Punctuator)
        std::cout << std::setw(35) << i.first << std::setw(35) << i.second << "\n";

    std::cout << std::setw(58) << "Constant\n";
    std::cout << std::setw(35) << "value" << std::setw(36) << "id\n\n";
    for (auto i : Constant)
        std::cout << std::setw(35) << i.first << std::setw(35) << i.second << "\n";

    std::cout << std::setw(58) << "Identifier\n";
    std::cout << std::setw(35) << "value" << std::setw(36) << "id\n\n";
    for (auto i : Identifier)
        std::cout << std::setw(35) << i.first << std::setw(35) << i.second << "\n";

    std::cout << std::setw(58) << "Character\n";
    std::cout << std::setw(35) << "value" << std::setw(36) << "id\n\n";
    for (auto i : Character)
        std::cout << std::setw(35) << i.first << std::setw(35) << i.second << "\n";

    std::cout << std::setw(58) << "String\n";
    std::cout << std::setw(35) << "value" << std::setw(36) << "id\n\n";
    for (auto i : String)
        std::cout << std::setw(35) << i.first << std::setw(35) << i.second << "\n";

    std::cout << std::setw(58) << "Array\n";
    std::cout << std::setw(35) << "value" << std::setw(36) << "id\n\n";
    for (auto i : Array)
        std::cout << std::setw(35) << i.first << std::setw(35) << i.second << "\n";

    std::cout << std::setw(58) << "Error\n";
    std::cout << std::setw(35) << "value" << std::setw(36) << "id\n\n";
    for (auto i : Error)
        std::cout << std::setw(35) << i.first << std::setw(35) << i.second << "\n";
}

void TokenSequence::printToken()
{
}

void TokenSequence::initKeyWord()
{
    std::ifstream infile;
    infile.open("../config/KeyWord.txt", std::ios::in);
    if (!infile.is_open())
    {
        std::cout << "KeyWord�ļ���ȡʧ��" << "\n";
        return;
    }
    std::string buf;
    while (getline(infile, buf))
    {
        KeyWord[buf] = IdKey++;
    }
}

void TokenSequence::initDelimiters()
{
    std::ifstream infile;
    infile.open("../config/Delimiters.txt", std::ios::in);
    if (!infile.is_open())
    {
        std::cout << "Delimiters�ļ���ȡʧ��" << "\n";
        return;
    }
    std::string buf;
    while (getline(infile, buf))
    {
        Punctuator[buf] = IdPunctuator++;
    }
}

int TokenSequence::get_kind(char ch)
{
    if (ch >= '0' && ch <= '9')
        return 1; // ����
    if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
        return 2; // ��ĸ
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '|' || ch == '^' || ch == '&' || ch == '>' || ch == '<' || ch == '=')
        return 3; // �����
    else if (ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == '(' || ch == ')' || ch == ';' || ch == '\'' || ch == '.')
        return 4; // �ָ���
    else
        return 5; // ��������
}

void TokenSequence::test()
{
    std::cout << "����test\n";
}

// int main()
// {
//     std::string s, tmp;

//     TokenSequence CinToken;

//     CinToken.initKeyWord();
//     CinToken.initDelimiters();
//     CinToken.scan();
//     CinToken.printAll();

//     vector<Token> Token_list = CinToken.getToken_list();

//     std::cout << std::setw(58) << "Token_list:\n";
//     std::cout << std::setw(25) << "type" << std::setw(25) << "value" << std::setw(25) << "id" << "\n\n";
//     for (auto i : Token_list)
//         std::cout << std::setw(25) << i.type << std::setw(25) << i.value << std::setw(25) << i.id << " \n";

//     return 0;
// }

// һЩ���۵�����
// int ab=1;  int a b =1;
// std::string a="  abwa "    ; std::string b="aw"abcd;
