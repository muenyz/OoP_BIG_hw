/*2253731 计科 张皓钦*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "hw_check.h"
#include "../include_mariadb_x86/mysql/mysql.h"

using namespace std;

int utfLen(char ch)
{
    if (!(ch & 0x80))
        return 1;
    if((ch & 0xE0) == 0xC0)
        return 2;
    if ((ch & 0xF0) == 0xE0)
        return 3;
    if ((ch & 0xF8) == 0xF0)
        return 4;
    if((ch & 0xFC) == 0xF8)
        return 5;
    if ((ch & 0xFE) == 0xFC)
        return 6;
    return 0;

}

int checkFileBase(const string dictory)
{
    ifstream fin(dictory);
    if (!fin) {
        return NO_SUBMIT;
    }
    else {
        if (fin.peek() == EOF) {
            fin.close();
            return FILE_EMPTY;
        }
        fin.close();
        fin.open(dictory, ios::ate);
        if(fin.tellg() < 3) {
            fin.close();
            return FILE_LESS_THAN_3B;
        }
        fin.seekg(0, ios::beg);
        string fex = dictory.substr(dictory.find_last_of('.'));
        if (fex == ".pdf") {
            char tmp[9] = { 0 };
            fin.seekg(0, ios::beg);
            if (fin.read(tmp, 8)) {
                if (string(tmp).substr(0, 7) == "%PDF-1." && tmp[7] >= '0' && tmp[7] <= '9') {
                    fin.close();
                    return CORRECT;
                }
                else {
                    fin.close();
                    return NOT_PDF;
                }
            }
            else {
                fin.close();
                return NOT_PDF;
            }
        }
        else if (fex == ".cpp" || fex == ".h" || fex == ".hpp" || fex == ".c") {
            string line;
            int flag1 = 0, flag2 = 1,flag3 = 0;
            
            fin.close();
            if(flag3 == 1&&flag2 == 1) {
                return NOT_GB;
            }
            else {
                return CORRECT;
            }
        }
        else {
            fin.close();
            return CORRECT;
        }
    }
}

int checkFileFirstLine(const string& firstline,const MYSQL_ROW& row,const string& stuNo,const string& stuName)
{
    string validParts;
    if (firstline.substr(0, 2) == "//") {
        validParts = firstline.substr(2);
    }
    else {
        validParts = firstline.substr(2, firstline.length() - 4);
    }
    stringstream ss(validParts);
    vector<string> parts;
    while (ss >> validParts) {
        parts.push_back(validParts);
    }
    if (parts.size() != 3) {
        cout << "首行不是三项 " << '[' << firstline << ']' << endl;
        return FIRSTLINE_NOT_3_ITEMS;
    }
    bool flag = true;//首行学号不匹配 姓名不匹配 班级不匹配  [/* 班级 学号 姓名 */]
    string stuCls = row[6];
    if (existInVector(parts, stuNo) == -1) {
        if (flag) {
            cout << "首行";
            flag = false;
        }
        cout << "学号不匹配 ";
    }
    if (existInVector(parts, stuName) == -1) {
        if (flag) {
            cout << "首行";
            flag = false;
        }
        cout << "姓名不匹配 ";
    }
    if (existInVector(parts, stuCls) == -1) {
        if (flag) {
            cout << "首行";
            flag = false;
        }
        cout << "班级不匹配 ";
    }
    if (flag) {
        cout << "正确" << endl;
        return CORRECT;
    }
    else {
        cout << " [" << firstline << "]" << endl;
        return FIRSTLINE_ERROR;
    }
}