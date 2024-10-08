/* 2253731 计科 张皓钦 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include "../include/class_aat.h"
//如有必要，可以加入其它头文件
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS //不使用lib才有效

/* ---------------------------------------------------------------
     允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */
static bool isValidArgFormat(const char* const arg)
{
    string str = arg;
    if (str.length() >= 2 && str.substr(0, 2) == "--")
        return true;
    else
        return false;
}

static int matchArg(const string arg, const args_analyse_tools* args)
{
    int index = 0;
    while (!args->get_name().empty())
    {
        if (arg == args->get_name())
            return index;
        index++;
        args++;
    }
    return -1;
}

static bool strIsInt(const string str)
{
    int flag = 0;
    int len = str.length();
    if (len == 0)
        return false;
    if(str[0] == '+' || str[0] == '-')
    {
        if (len == 1)
            return false;
        flag = 1;
    }
    for (int i = flag;i < len;i++)
    {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

static bool IPisValid(const char* IP)
{
    //3个点，4个0-255的整数，两点间必须有数字，两头必须有数字，数字可以有前导0，但值要是0-255
    int point[5] = { -1,0, 0, 0,(int)strlen(IP) }, pointCnt = 0;//这里虚设两个点，方便后面的循环
    for (int i = 0;IP[i];i++)
        if (IP[i] == '.') {
            if (pointCnt == 3)//如果已经有3个点了，就不合法
                return false;
            else
                point[++pointCnt] = i;
        }
    if (pointCnt != 3)
        return false;
    //检查每个点之间的数字
    for (int i = 0;i <= 3;i++) {
        if (point[i + 1] - point[i] <= 1)//如果两点之间没有数字
            return false;
        for (const char* p = IP + point[i] + 1;p != IP + point[i + 1];p++)
            if (*p < '0' || *p>'9')
                return false;
        if (atoi(IP + point[i] + 1) > 255)//如果数字不在0-255之间，其中负号已经过不了前面那关，所以只需要考虑255
            return false;
    }
    return true;
}

static unsigned int IPtoUINT(const char* IP)
{
    unsigned int ret = 0;
    int point[5] = { -1,0, 0, 0,(int)strlen(IP) }, pointCnt = 0;//这里虚设两个点，方便后面的循环
    for (int i = 0;IP[i];i++)
        if (IP[i] == '.') {
            point[++pointCnt] = i;
        }
    for (int i = 0;i <= 3;i++) {
        ret |= atoi(IP + point[i] + 1) << (3 - i) * 8;
    }
    return ret;
}

static void printTable(int row, int col, const string* const table)
{
    cout << left;
    unsigned int* colWidth = new unsigned int[col];
    if(colWidth == NULL)
        exit(-1);
    for (int i = 0;i < col;i++)
        colWidth[i] = 0;
    for (int i = 0;i < row;i++)
        for (int j = 0;j < col;j++)
            if (table[i * col + j].length() > colWidth[j])
                colWidth[j] = table[i * col + j].length();
    //先输出表头
    cout << setfill('=');
    for (int i = 0;i < col;i++) {
        cout <<setw(colWidth[i]) << "";
        if (i != col - 1)
            cout << "=";
    }
    cout << setfill(' ') << endl;
    for (int i = 0;i < col;i++) {
        cout  << setw(colWidth[i]) << table[i];
        if (i != col - 1)
            cout << " ";
    }
    cout << setfill('=') << endl;
    for (int i = 0;i < col;i++) {
        cout << setw(colWidth[i]) << "";
        if (i != col - 1)
            cout << "=";
    }
    cout << setfill(' ') << endl;
    for (int i = 1;i < row;i++) {
        for (int j = 0;j < col;j++) {
            cout << setw(colWidth[j]) << table[i * col + j];
            if (j != col - 1)
                cout << " ";
        }
        cout << endl;
    }
    cout << setfill('=');
    for (int i = 0;i < col;i++) {
        cout << setw(colWidth[i]) << "";
        if (i != col - 1)
            cout << "=";
    }
    cout << setfill(' ') << endl;
    cout<<right<<endl;
    delete[] colWidth;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
    args_name = "";
    extargs_type = ST_EXTARGS_TYPE::null;
    extargs_num = 0;

}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
    args_name = name;
    extargs_type = type;
    extargs_num = ext_num;
    extargs_bool_default = def;
    args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_default、int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
    args_name = name;
    extargs_type = type;
    extargs_num = ext_num;
    extargs_int_value = extargs_int_default = (def<_min || def>_max ? _min : def);
    extargs_int_min = _min;
    extargs_int_max = _max;
    args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_set_default、int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
    args_name = name;
    extargs_type = type;
    extargs_num = ext_num;
    int cnt = 0;
    while (set[cnt++] != INVALID_INT_VALUE_OF_SET)
        ;
    extargs_int_set = new int[cnt];
    if (extargs_int_set == NULL)
        exit(-1);
    for (int i = 0;i < cnt;i++)
        extargs_int_set[i] = set[i];
    extargs_int_value=extargs_int_default = (def_of_set_pos>=0&&def_of_set_pos<cnt-1?set[def_of_set_pos]:set[0]);
    args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str、ipaddr_with_default、ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
    args_name = name;
    extargs_type = type;
    extargs_num = ext_num;
    if(type == ST_EXTARGS_TYPE::ipaddr_with_default)
        extargs_ipaddr_value=extargs_ipaddr_default = IPisValid(def.c_str()) ? IPtoUINT(def.c_str()) : 0x7f000001;
    else if(type == ST_EXTARGS_TYPE::ipaddr_with_error)
        extargs_ipaddr_value = extargs_ipaddr_default = 0x7f000001;
    else
        extargs_string_value= extargs_string_default = def;
    args_existed = 0;
    
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str_with_set_default、str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
    args_name = name;
    extargs_type = type;
    extargs_num = ext_num;
    int cnt = 0;
    while (!set[cnt++].empty())
        ;
    extargs_string_set = new string[cnt];
    if (extargs_string_set == NULL)
        exit(-1);
    for (int i = 0;i < cnt;i++)
        extargs_string_set[i] = set[i];
    extargs_string_value = extargs_string_default = (def_of_set_pos >= 0 && def_of_set_pos < cnt - 1 ? set[def_of_set_pos] : set[0]);
    args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
    if((extargs_type == ST_EXTARGS_TYPE::int_with_set_default || extargs_type == ST_EXTARGS_TYPE::int_with_set_error) && extargs_int_set != NULL)
        delete[] extargs_int_set;
    if ((extargs_type == ST_EXTARGS_TYPE::str_with_set_default || extargs_type == ST_EXTARGS_TYPE::str_with_set_error) && extargs_string_set != NULL)
        delete[] extargs_string_set;
}

/* ---------------------------------------------------------------
     允许AAT中自定义成员函数的实现（private）
   ---------------------------------------------------------------- */

   /***************************************************************************
     函数名称：
     功    能：
     输入参数：
     返 回 值：
     说    明：已实现，不要动
    ***************************************************************************/
const string args_analyse_tools::get_name() const
{
    return this->args_name;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：加!!后，只能是0/1
            已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
    return !!this->args_existed;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
    return this->extargs_int_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
    return this->extargs_string_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
    return this->extargs_ipaddr_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：将 extargs_ipaddr_value 的值从 0x7f000001 转为 "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
    return to_string(((extargs_ipaddr_value) >> 3 * 8) & ((1 << 8) - 1)) + "." + to_string(((extargs_ipaddr_value) >> 2 * 8) & ((1 << 8) - 1)) + "." + to_string(((extargs_ipaddr_value) >> 1 * 8) & ((1 << 8) - 1)) + "." + to_string(((extargs_ipaddr_value) >> 0 * 8) & ((1 << 8) - 1)); //此句根据需要修改
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：follow_up_args：是否有后续参数
            0  ：无后续参数
            1  ：有后续参数
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_process(const int argc, const char* const* const argv, args_analyse_tools* const args, const int follow_up_args)
{
    int cnt = 1;
    for (int i = 1;i < argc;i++,cnt++) {
        if (isValidArgFormat(argv[i])) {
            int index = matchArg(argv[i], args);
            if (index == -1) {
                if (follow_up_args)
                    return cnt;
                else if (string(argv[i]) == "--") {
                    cout << "参数[--]格式非法(不是--开头的有效内容)." << endl;
                    return -1;
                }
                else {
                    cout << "参数[" << argv[i] << "]非法." << endl;
                    return -1;
                }
            }//格式正确但匹配不到
            if (args[index].args_existed) {
                cout << "参数[" << argv[i] << "]重复." << endl;
                return -1;
            }//重复出现
            args[index].args_existed = 1;
            if (args[index].extargs_num == 0)
                continue;
            else if (i == argc - 1 || isValidArgFormat(argv[i + 1])) {
                cout << "参数[" << argv[i] << "]" << (i == argc - 1?"的附加参数不足":"缺少附加参数") << ". (类型:";
                switch (args[index].extargs_type)
                {
                    case ST_EXTARGS_TYPE::int_with_default:
                    case ST_EXTARGS_TYPE::int_with_error:
                        cout << "int, 范围[" << args[index].extargs_int_min << ".." << args[index].extargs_int_max << "]";
                        if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
                            ;
                        else
                            cout << " 缺省:" << args[index].extargs_int_default;
                        break;
                    case ST_EXTARGS_TYPE::int_with_set_default:
                    case ST_EXTARGS_TYPE::int_with_set_error:
                        cout << "int, 范围[";
                        for (const int* p = args[index].extargs_int_set;*p != INVALID_INT_VALUE_OF_SET;) {
                            cout << *p++;
                            cout << (*p == INVALID_INT_VALUE_OF_SET ? "]" : "/");
                        }
                        if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
                            ;
                        else
                            cout << " 缺省:" << args[index].extargs_int_default;
                        break;
                    case ST_EXTARGS_TYPE::str:
                        cout << "string";
                        if (!args[index].extargs_string_default.empty())
                            cout << " 缺省:" << args[index].extargs_string_default;
                        break;
                    case ST_EXTARGS_TYPE::ipaddr_with_default:
                    case ST_EXTARGS_TYPE::ipaddr_with_error:
                        cout << "IP地址";
                        if (args[index].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
                            ;
                        else
                            cout << " 缺省:" << args[index].get_str_ipaddr();
                        break;
                    case ST_EXTARGS_TYPE::str_with_set_default:
                    case ST_EXTARGS_TYPE::str_with_set_error:
                        cout << "string, 可取值[";
                        for (const string* p = args[index].extargs_string_set;!p->empty();) {
                            cout << *p++;
                            cout << (p->empty() ? "]" : "/");
                        }
                        if (args[index].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
                            ;
                        else
                            cout << " 缺省:" << args[index].extargs_string_default;
                }
                cout << ")" << endl;
                return -1;
            }//缺少附加参数
            else {
                switch (args[index].extargs_type)
                {
                    case ST_EXTARGS_TYPE::int_with_default:
                    case ST_EXTARGS_TYPE::int_with_error:
                    {
                        if (!strIsInt(argv[i + 1])) {
                            cout << "参数[" << argv[i] << "]的附加参数不是整数. (类型:int, 范围[" << args[index].extargs_int_min << ".." << args[index].extargs_int_max << "]";
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
                                cout << ")" << endl;
                            else
                                cout << " 缺省:" << args[index].extargs_int_default << ")" << endl;
                            return -1;
                        }
                        int value1 = atoi(argv[i + 1]);
                        if (value1 < args[index].extargs_int_min || value1 > args[index].extargs_int_max) {
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_error) {
                                cout << "参数[" << argv[i] << "]的附加参数值(" << value1 << ")非法. (类型:int, 范围[" << args[index].extargs_int_min << ".." << args[index].extargs_int_max << "])" << endl;
                                return -1;
                            }
                            else
                                value1 = args[index].extargs_int_default;
                        }
                        args[index].extargs_int_value = value1;
                        break;
                    }
                    case ST_EXTARGS_TYPE::int_with_set_default:
                    case ST_EXTARGS_TYPE::int_with_set_error:
                    {
                        if (!strIsInt(argv[i + 1])) {
                            cout << "参数[" << argv[i] << "]的附加参数不是整数. (类型:int, 范围[";
                            for (const int* p = args[index].extargs_int_set;*p != INVALID_INT_VALUE_OF_SET;) {
                                cout << *p++;
                                cout << (*p == INVALID_INT_VALUE_OF_SET ? "]" : "/");
                            }
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
                                cout << ")" << endl;
                            else
                                cout << " 缺省:" << args[index].extargs_int_default << ")" << endl;
                            return -1;
                        }
                        int value2 = atoi(argv[i + 1]);
                        bool flag = false;
                        for (const int* p = args[index].extargs_int_set;*p != INVALID_INT_VALUE_OF_SET;p++) {
                            if (*p == value2) {
                                flag = true;
                                break;
                            }
                        }
                        if (!flag) {
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_set_error) {
                                cout << "参数[" << argv[i] << "]的附加参数值(" << value2 << ")非法. (类型:int, 范围[";
                                for (const int* p = args[index].extargs_int_set;*p != INVALID_INT_VALUE_OF_SET;) {
                                    cout << *p++;
                                    cout << (*p == INVALID_INT_VALUE_OF_SET ? "]" : "/");
                                }
                                cout << ")" << endl;
                                return -1;
                            }
                            else
                                value2 = args[index].extargs_int_default;
                        }
                        args[index].extargs_int_value = value2;
                        break;
                    }
                    case ST_EXTARGS_TYPE::str:
                        if (isValidArgFormat(argv[i + 1])) {
                            cout << "参数[" << argv[i] << "]缺少附加参数. (类型:string";
                            if (!args[index].extargs_string_default.empty())
                                cout << " 缺省:" << args[index].extargs_string_default << ")" << endl;
                            else
                                cout << ")" << endl;
                            return -1;
                        }
                        args[index].extargs_string_value = argv[i + 1];
                        break;
                    case ST_EXTARGS_TYPE::str_with_set_default:
                    case ST_EXTARGS_TYPE::str_with_set_error:
                    {
                        if (isValidArgFormat(argv[i + 1])) {
                            cout << "参数[" << argv[i] << "]的附加参数不足. (类型:string, 可取值[";
                            for (const string* p = args[index].extargs_string_set;!p->empty();) {
                                cout << *p++;
                                cout << (p == args[index].extargs_string_set ? "]" : "/");
                            }
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
                                cout << ")" << endl;
                            else
                                cout << " 缺省:" << args[index].extargs_string_default << ")" << endl;
                            return -1;
                        }
                        string strvalue = argv[i + 1];
                        bool flag = false;
                        for (const string* p = args[index].extargs_string_set;!p->empty();p++) {
                            if (*p == strvalue) {
                                flag = true;
                                break;
                            }
                        }
                        if (!flag) {
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::str_with_set_error) {
                                cout << "参数[" << argv[i] << "]的附加参数值(" << argv[i + 1] << ")非法. (类型:string, 可取值[";
                                for (const string* p = args[index].extargs_string_set;!p->empty();) {
                                    cout << *p++;
                                    cout << (p == args[index].extargs_string_set ? "]" : "/");
                                }
                                cout << ")" << endl;
                                return -1;
                            }
                            else
                                strvalue = args[index].extargs_string_default;
                        }
                        args[index].extargs_string_value = strvalue;
                        break;
                    }
                    case ST_EXTARGS_TYPE::ipaddr_with_default:
                    case ST_EXTARGS_TYPE::ipaddr_with_error:
                        if (!IPisValid(argv[i + 1])) {
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error) {
                                cout << "参数[" << argv[i] << "]的附加参数值(" << argv[i + 1] << ")非法. (类型:IP地址)";
                                return -1;
                            }
                            else
                                args[index].extargs_ipaddr_value = args[index].extargs_ipaddr_default;
                        }
                        else
                            args[index].extargs_ipaddr_value = IPtoUINT(argv[i + 1]);
                        break;
                }
                i++,cnt++;
            }
        }
        else if (follow_up_args)
            return cnt;
        else {
            cout << "参数[" << argv[i] << "]格式非法(不是--开头的有效内容)." << endl;
            return -1;
        }
    }
    return cnt; //此句根据需要修改
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
    int line =0;
    while (!args[line++].args_name.empty())//有个头行，这里使line刚好大1放它
        ;
    string* table = new string[line * 6]{ " name","type","default","exists","value","range/set " };
    if (table == NULL)
        exit(-1);

    for (int i = 1;i < line;i++) {
        table[i * 6 + 0] = " " + args[i - 1].get_name();
        switch(args[i-1].extargs_type)
        {
            case ST_EXTARGS_TYPE::boolean:
                table[i * 6 + 1] = "Bool";
                table[i * 6 + 2] = args[i - 1].extargs_bool_default ? "true" : "false";
                if (args[i - 1].existed()) {
                    table[i * 6 + 3] = "1";
                    table[i * 6 + 4] = "true";
                }
                else {
                    table[i * 6 + 3] = "0";
                    table[i * 6 + 4] = "/";
                }
                table[i * 6 + 5] = "/ ";
                break;
            case ST_EXTARGS_TYPE::int_with_default:
                table[i * 6 + 1] = "IntWithDefault";
                table[i * 6 + 2] = to_string(args[i - 1].extargs_int_default);
                if (args[i - 1].existed()) {
                    table[i * 6 + 3] = "1";
                    table[i * 6 + 4] = to_string(args[i - 1].extargs_int_value);
                }
                else {
                    table[i * 6 + 3] = "0";
                    table[i * 6 + 4] = "/";
                }
                table[i * 6 + 5] = "[" + to_string(args[i - 1].extargs_int_min) + ".." + to_string(args[i - 1].extargs_int_max) + "] ";
                break;
            case ST_EXTARGS_TYPE::int_with_error:
                table[i * 6 + 1] = "IntWithError";
                table[i * 6 + 2] = "/";
                if (args[i - 1].existed()) {
                    table[i * 6 + 3] = "1";
                    table[i * 6 + 4] = to_string(args[i - 1].extargs_int_value);
                }
                else {
                    table[i * 6 + 3] = "0";
                    table[i * 6 + 4] = "/";
                }
                table[i * 6 + 5] = "[" + to_string(args[i - 1].extargs_int_min) + ".." + to_string(args[i - 1].extargs_int_max) + "] ";
                break;
            case ST_EXTARGS_TYPE::int_with_set_default:
                table[i * 6 + 1] = "IntSETWithDefault";
                table[i * 6 + 2] = to_string(args[i - 1].extargs_int_default);
                if (args[i - 1].existed()) {
                    table[i * 6 + 3] = "1";
                    table[i * 6 + 4] = to_string(args[i - 1].extargs_int_value);
                }
                else {
                    table[i * 6 + 3] = "0";
                    table[i * 6 + 4] = "/";
                }
                for (const int* p = args[i - 1].extargs_int_set;*p != INVALID_INT_VALUE_OF_SET;) {
                    table[i * 6 + 5] += to_string(*p++);
                    table[i * 6 + 5] += (*p == INVALID_INT_VALUE_OF_SET ? " " : "/");
                }
                break;
            case ST_EXTARGS_TYPE::int_with_set_error:
                table[i * 6 + 1] = "IntSETWithError";
                table[i * 6 + 2] = "/";
                if (args[i - 1].existed()) {
                    table[i * 6 + 3] = "1";
                    table[i * 6 + 4] = to_string(args[i - 1].extargs_int_value);
                }
                else {
                    table[i * 6 + 3] = "0";
                    table[i * 6 + 4] = "/";
                }
                for (const int* p = args[i - 1].extargs_int_set;*p != INVALID_INT_VALUE_OF_SET;) {
                    table[i * 6 + 5] += to_string(*p++);
                    table[i * 6 + 5] += (*p == INVALID_INT_VALUE_OF_SET ? " " : "/");
                }
                break;
            case ST_EXTARGS_TYPE::str:
                table[i * 6 + 1] = "String";
                table[i * 6 + 2] = (!args[i - 1].extargs_string_default.empty() ? args[i - 1].extargs_string_default : "/");
                if (args[i - 1].existed()) {
                    table[i * 6 + 3] = "1";
                    table[i * 6 + 4] = args[i - 1].extargs_string_value;
                }
                else {
                    table[i * 6 + 3] = "0";
                    table[i * 6 + 4] = "/";
                }
                table[i * 6 + 5] = "/ ";
                break;
            case ST_EXTARGS_TYPE::str_with_set_default:
                table[i * 6 + 1] = "StringSETWithDefault";
                table[i * 6 + 2] = args[i - 1].extargs_string_default;
                if (args[i - 1].existed()) {
                    table[i * 6 + 3] = "1";
                    table[i * 6 + 4] = args[i - 1].extargs_string_value;
                }
                else {
                    table[i * 6 + 3] = "0";
                    table[i * 6 + 4] = "/";
                }
                for (const string* p = args[i - 1].extargs_string_set;!p->empty();) {
                    table[i * 6 + 5] += *p++;
                    table[i * 6 + 5] += (p->empty() ? " " : "/");
                }
                break;
            case ST_EXTARGS_TYPE::str_with_set_error:
                table[i * 6 + 1] = "StringSETWithError";
                table[i * 6 + 2] = "/";
                if (args[i - 1].existed()) {
                    table[i * 6 + 3] = "1";
                    table[i * 6 + 4] = args[i - 1].extargs_string_value;
                }
                else {
                    table[i * 6 + 3] = "0";
                    table[i * 6 + 4] = "/";
                }
                for (const string* p = args[i - 1].extargs_string_set;!p->empty();) {
                    table[i * 6 + 5] += *p++;
                    table[i * 6 + 5] += (p->empty() ? " " : "/");
                }
                break;
            case ST_EXTARGS_TYPE::ipaddr_with_default:
                table[i * 6 + 1] = "IPAddrWithDefault";
                table[i * 6 + 2] = args[i - 1].get_str_ipaddr();
                if (args[i - 1].existed()) {
                    table[i * 6 + 3] = "1";
                    table[i * 6 + 4] = args[i - 1].get_str_ipaddr();
                }
                else {
                    table[i * 6 + 3] = "0";
                    table[i * 6 + 4] = "/";
                }
                table[i * 6 + 5] = "/ ";
                break;
            case ST_EXTARGS_TYPE::ipaddr_with_error:
                table[i * 6 + 1] = "IPAddrWithError";
                table[i * 6 + 2] = "/";
                if (args[i - 1].existed()) {
                    table[i * 6 + 3] = "1";
                    table[i * 6 + 4] = args[i - 1].get_str_ipaddr();
                }
                else {
                    table[i * 6 + 3] = "0";
                    table[i * 6 + 4] = "/";
                }
                table[i * 6 + 5] = "/ ";
                break;
        }
    }
    printTable(line, 6, table);
    delete[] table;
    return 0; //此句根据需要修改
}

#endif // !ENABLE_LIB_COMMON_TOOLS
