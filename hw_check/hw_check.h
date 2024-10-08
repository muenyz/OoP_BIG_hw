/*2253731 ¼Æ¿Æ ÕÅð©ÇÕ*/
#pragma once
#include <iostream>
#include <vector>
#include "../include_mariadb_x86/mysql/mysql.h"

using namespace std;
const int CORRECT = 0;
const int ERROR = 1;
const int NO_SUBMIT = 2;
const int FILE_LESS_THAN_3B = 3;
const int NOT_GB = 4;
const int NOT_PDF = 5;
const int NO_VALID_FIRSTLINE = 6;
const int FIRSTLINE_NOT_COMMENT = 7;
const int FIRSTLINE_NOT_3_ITEMS = 8;
const int FIRSTLINE_ERROR = 9;
const int SECONDLINE_NOT_COMMENT = 10;
const int FILE_EMPTY = 11;
const int SECONDLINE_OTHER_ERROR = 12;
const int FIRSTLINE_CMT_IN_DIFF_LINE = 13;
const int STATUS_NUM = 14;

class VeriList {
public:
    string stuNo;
    string stuName;
    vector<string> mates;
    bool operator==(const VeriList& v) {
        return stuNo == v.stuNo;
    }
    VeriList(string stuNo = "", string stuName = "") :stuNo(stuNo), stuName(stuName)
    {
    }
};

template <typename T>
int existInVector(vector<T> v, T t)
{
    for (unsigned int i = 0;i < v.size();i++) {
        if (v[i] == t)
            return i;
    }
    return -1;
}



int checkFileBase(const string dictory);
int checkFileFirstLine(const string& firstline, const MYSQL_ROW& row, const string& stuNo, const string& stuName);