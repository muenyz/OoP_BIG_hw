/* 2253731 计科 张皓钦 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "../include/class_crt.h"
using namespace std;

/* 允许加入需要的头文件，给出 cfgfile_read_tools类的实现 */
#include <string>
#include <fstream>
#include "../include/linestring.h"

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
cfgfile_read_tools::cfgfile_read_tools(const char* _cfgname)
{
    ifstream fin(_cfgname);
    if (fin.is_open()) {
        cfgname = _cfgname;
        string line;
        while(getline(fin, line)){
            getValidLine(line);
            if(isGroupName(line)||line.find('=')!=string::npos)
                cfgFile.push_back(line);
        }
        isOpen = true;
        fin.close();
    }
    else
        isOpen = false;
}

cfgfile_read_tools::cfgfile_read_tools(const string& _cfgname)
{
    ifstream fin(_cfgname);
    if (fin.is_open()) {
        cfgname = _cfgname;
        string line;
        while (getline(fin, line)) {
            getValidLine(line);
            cfgFile.push_back(line);
        }
        isOpen = true;
        fin.close();
    }
    else
        isOpen = false;
}

cfgfile_read_tools::~cfgfile_read_tools()
{
}

bool cfgfile_read_tools::is_open()
{
    return isOpen;
}

bool cfgfile_read_tools::isGroupName(const string& str)
{
    if (str.empty())
        return false;
    if (str[0] == '[' && str[str.size() - 1] == ']')
        return true;
    return false;
}

void cfgfile_read_tools::getValidLine(string& str)
{
    cutComment(str);
    trimLine(str, "all");
    return;
}

int cfgfile_read_tools::get_all_group(vector <string>& ret)
{
    if (!isOpen)
        return -1;
    int cnt = 0;
    for(string line : cfgFile){
        if (isGroupName(line)){
            ret.push_back(line);
            cnt++;
        }
    }

    return cnt;
}

int cfgfile_read_tools::get_all_item(const string& group_name, vector <string>& ret, const bool ignore_lower_upper_case)
{
    if (!isOpen)
        return -1;
    int cnt = 0;
    const bool simpleCFG = group_name.empty();
    bool isGroup = simpleCFG;
    for (string line : cfgFile) {
        getValidLine(line);
        if (isGroupName(line)) {
            if (simpleCFG)
                continue;
            if (isGroup)
                break;
            if (ignore_lower_upper_case) {
                if (_strcmpi(line.c_str(), group_name.c_str()) == 0)
                    isGroup = true;
            }
            else {
                if (group_name == line)
                    isGroup = true;
            }
        }
        else if (isGroup) {
            if (line.find('=') != string::npos) {
                ret.push_back(line);
                cnt++;
            }
        }
    }

    return isGroup ? cnt : -1;
}

int cfgfile_read_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool ignore_lower_upper_case)
{
    if(group_name)
        return get_all_item(string(group_name), ret, ignore_lower_upper_case);
    else
        return get_all_item(string(""), ret, ignore_lower_upper_case);
}

int cfgfile_read_tools::get_one_item(const string& group_name, const string& item_name, string &ret, const bool ignore_lower_upper_case)
{
    if (!isOpen)
        return -1;
    bool isGroup = false;
    bool flag = false;
    for (string line : cfgFile) {
        getValidLine(line);
        if (isGroupName(line)) {
            if (isGroup)
                break;
            if (ignore_lower_upper_case) {
                if (_strcmpi(line.c_str(), group_name.c_str()) == 0)
                    isGroup = true;
            }
            else {
                if (group_name == line)
                    isGroup = true;
            }
        }
        else if (isGroup) {
            if (line.find('=') != string::npos) {
                string item = line.substr(0, line.find('='));
                trimLine(item, "all");
                if (ignore_lower_upper_case) {
                    if (_strcmpi(item.c_str(), item_name.c_str()) == 0) {
                        ret = line;
                        flag = true;
                        break;
                    }
                }
                else {
                    if (item == item_name) {
                        ret = line;
                        flag = true;
                        break;
                    }
                }
            }
        }
    }

    return flag ? 0 : -1;
}

int cfgfile_read_tools::get_one_item(const char* const group_name, const char* const item_name, string &ret, const bool ignore_lower_upper_case)
{
    return get_one_item(string(group_name), string(item_name), ret, ignore_lower_upper_case);
}

int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, char* str, const int str_maxlen, const bool ignore_lower_upper_case)
{
    if (!isOpen)
        return -1;
    string ret;
    int flag = get_one_item(group_name, item_name, ret, ignore_lower_upper_case);
    if (flag == 0) {
        string tmp = ret.substr(ret.find('=') + 1);
        trimLine(tmp, "all");
        cutComment(tmp, " \t");
        if (tmp.empty())
            tmp = def_str;
		strcpy(str, tmp.length() >= (unsigned)str_maxlen ? tmp.substr(0, str_maxlen - 1).c_str() : tmp.c_str());
    }
    else {
        return -1;
    }

    return 0;
}

int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, string& value, const bool ignore_lower_upper_case)
{
    if (!isOpen)
        return -1;
    string ret;
    int flag = get_one_item(group_name, item_name, ret, ignore_lower_upper_case);
    if (flag == 0) {
        string tmp = ret.substr(ret.find('=') + 1);
        trimLine(tmp, "all");
        if (tmp.empty())
            value = def_str;
        else
            value = tmp;
    }
    else {
        return -1;
    }

    return 0;
}

int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, char &ch, const bool ignore_lower_upper_case)
{
    if (!isOpen)
        return -1;
    string ret;
    int flag = get_one_item(group_name, item_name, ret, ignore_lower_upper_case);
    if (flag == 0) {
        string tmp = ret.substr(ret.find('=') + 1);
        trimLine(tmp, "all");
        if (tmp.empty())
            ch = ' ';
        else
            ch = tmp[0];
    }
    else {
        return -1;
    }

    return 0;
}

int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const int min_value, const int max_value, const int def_value, int& value, const bool ignore_lower_upper_case)
{
    if (!isOpen)
        return -1;
    string ret;
    int flag = get_one_item(group_name, item_name, ret, ignore_lower_upper_case);
    if (flag == 0) {
        string tmp = ret.substr(ret.find('=') + 1);
        trimLine(tmp, "all");
        cutComment(tmp, " \t");
        if (tmp.empty())
            value = def_value;
        else {
            try {
                value = stoi(tmp);
            }
            catch (...) {
                value = def_value;
            }
        }
        if (value < min_value || value > max_value)
            value = def_value;
    }
    else {
        return -1;
    }

    return 0;
}

int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const double min_value, const double max_value, const double def_value, double& value, const bool ignore_lower_upper_case)
{
    if (!isOpen)
        return -1;
    string ret;
    int flag = get_one_item(group_name, item_name, ret, ignore_lower_upper_case);
    if (flag == 0) {
        string tmp = ret.substr(ret.find('=') + 1);
        trimLine(tmp, "all");
        cutComment(tmp, " \t");
        if (tmp.empty())
            value = def_value;
        else {
            try {
                value = stod(tmp);
            }
            catch (...) {
                value = def_value;
            }
        }
        if (value < min_value || value > max_value)
            value = def_value;
    }
    else {
        return -1;
    }

    return 0;
}
