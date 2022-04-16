#include<stdio.h>
#include<string.h>
#include<iostream>
#include<fstream>
#include <unordered_map>
using namespace std;


unordered_map<string, string> constMap = {
    {"const","CONSTTK"},
    {"int", "INTTK"},
    {"char", "CHARTK"},
    {"void", "VOIDTK"},
    {"main", "MAINTK"},
    {"printf", "PRINTFTK"},
    {"return", "RETURNTK"},
    {"if", "IFTK"},
    {"else", "ELSETK"},
    {"do", "DOTK"},
    {"while", "WHILETK"},
    {"for", "FORTK"},
    {"scanf", "SCANFTK"}
};
unordered_map<string, string> operatorMap = {
    {"+", "PLUS"},
    {"-", "MINU"},
    {"*", "MULT"},
    {"/", "DIV"},
    {"<", "LSS"},
    {">", "GRE"},
    {"<=", "LEQ"},
    {">=", "GEQ"},
    {"==", "EQL"},
    {"!=", "NEQ"},
    {"=", "ASSIGN"}
};
unordered_map<string, string> separatorMap = {
    {";", "SEMICN"},
    {",", "COMMA"},
    {"(", "LPARENT"},
    {")", "RPARENT"},
    {"[", "LBRACK"},
    {"]", "RBRACK"},
    {"{", "LBRACE"},
    {"}", "RBRACE"}
};

int main() {
    ifstream ifs;
    ifs.open("testfile.txt", ios::in);
    ofstream ofs;
    ofs.open("output.txt", ios::out | ios::trunc);

    if (!ifs.is_open())
    {
        cout << "read fail." << endl;
        return -1;
    }

    char c;
    ifs >> noskipws;    // 不忽略空格与换行
    int index = 0;
    while (ifs.peek() != EOF)
    {
        char cList[50] = "";
        string ident;
        string sym;
        bool isOver = false;    // 用来防止文件最后一行出现死循环
        ifs >> c;
        while (isspace(c) || c == '\n' || c == '\0' || c == '\t') {
            if (ifs.peek() == EOF) {
                isOver = true;
                break;
            }
            ifs.seekg(++index);
            ifs >> c;
        }
        if (isOver) break;
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
            int k = 0;
            do  // 标识符或者保留字
            {
                if (k < 50) {
                    cList[k] = c;
                    k++;
                }
                ifs.seekg(++index);
                ifs >> c;
            } while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_');
            ifs.seekg(--index);
            ident = cList;
            // 判断是否为保留字
            bool ifConst = false;
            if (constMap.count(ident) != 0) {
                ifConst = true;
            }
            if (ifConst) {
                sym = constMap[ident];
            }
            else {
                sym = "IDENFR";
            }
        }
        else {
            if (c >= '0' && c <= '9') { // 不是字母是数字
                // 拼数字，得出整形常量
                int k = 0;
                while (c >= '0' && c <= '9') {
                    cList[k] = c;
                    ifs.seekg(++index);
                    ifs >> c;
                    k++;
                }
                ifs.seekg(--index);
                ident = cList;
                sym = "INTCON";
            }
            else {  // 不是字母也不是数字
                if (c == '\'') {    // 不是字母不是数字，是字符常量
                    ifs.seekg(++index);
                    ifs >> c;
                    ident = c;
                    sym = "CHARCON";
                    ifs.seekg(++index); // 把另一个引号去掉
                }
                else if (c == '"') {    // 不是字母不是数字，是字符串常量
                    ifs.seekg(++index);
                    ifs >> c;
                    int k = 0;
                    while (c != '"') {
                        cList[k] = c;
                        ifs.seekg(++index);
                        ifs >> c;
                        k++;
                    }
                    ident = cList;
                    sym = "STRCON";
                }
                else {  // 不是字母不是数字，是运算符或者分隔符
                    string s(1, c);
                    if (separatorMap.count(s)) {    // 是分隔符
                        ident = c;
                        sym = separatorMap[ident];
                    }
                    else {  // 是运算符
                        int k = 0;
                        while (!isspace(c) && !isdigit(c) && c != '\n' && !isalpha(c) && c != '\'') {
                            cList[k] = c;
                            ifs.seekg(++index);
                            ifs >> c;
                            k++;
                        }
                        ifs.seekg(--index);
                        ident = cList;
                        sym = operatorMap[ident];
                    }
                }
            }
        }
        ifs.seekg(++index);
        if (ifs.peek() == EOF) {
            ofs << sym << " " << ident;
        }
        else {
            ofs << sym << " " << ident << endl;
        }
    }
    ifs.close();
    ofs.close();
    return 0;
}