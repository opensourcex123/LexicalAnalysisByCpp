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
    ifs >> noskipws;    // �����Կո��뻻��
    int index = 0;
    while (ifs.peek() != EOF)
    {
        char cList[50] = "";
        string ident;
        string sym;
        bool isOver = false;    // ������ֹ�ļ����һ�г�����ѭ��
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
            do  // ��ʶ�����߱�����
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
            // �ж��Ƿ�Ϊ������
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
            if (c >= '0' && c <= '9') { // ������ĸ������
                // ƴ���֣��ó����γ���
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
            else {  // ������ĸҲ��������
                if (c == '\'') {    // ������ĸ�������֣����ַ�����
                    ifs.seekg(++index);
                    ifs >> c;
                    ident = c;
                    sym = "CHARCON";
                    ifs.seekg(++index); // ����һ������ȥ��
                }
                else if (c == '"') {    // ������ĸ�������֣����ַ�������
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
                else {  // ������ĸ�������֣�����������߷ָ���
                    string s(1, c);
                    if (separatorMap.count(s)) {    // �Ƿָ���
                        ident = c;
                        sym = separatorMap[ident];
                    }
                    else {  // �������
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