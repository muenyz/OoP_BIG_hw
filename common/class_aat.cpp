/* 2253731 �ƿ� ����� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include "../include/class_aat.h"
//���б�Ҫ�����Լ�������ͷ�ļ�
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS //��ʹ��lib����Ч

/* ---------------------------------------------------------------
     �������������Ҫstatic�������ڲ������ã�
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
    //3���㣬4��0-255���������������������֣���ͷ���������֣����ֿ�����ǰ��0����ֵҪ��0-255
    int point[5] = { -1,0, 0, 0,(int)strlen(IP) }, pointCnt = 0;//�������������㣬��������ѭ��
    for (int i = 0;IP[i];i++)
        if (IP[i] == '.') {
            if (pointCnt == 3)//����Ѿ���3�����ˣ��Ͳ��Ϸ�
                return false;
            else
                point[++pointCnt] = i;
        }
    if (pointCnt != 3)
        return false;
    //���ÿ����֮�������
    for (int i = 0;i <= 3;i++) {
        if (point[i + 1] - point[i] <= 1)//�������֮��û������
            return false;
        for (const char* p = IP + point[i] + 1;p != IP + point[i + 1];p++)
            if (*p < '0' || *p>'9')
                return false;
        if (atoi(IP + point[i] + 1) > 255)//������ֲ���0-255֮�䣬���и����Ѿ�������ǰ���ǹأ�����ֻ��Ҫ����255
            return false;
    }
    return true;
}

static unsigned int IPtoUINT(const char* IP)
{
    unsigned int ret = 0;
    int point[5] = { -1,0, 0, 0,(int)strlen(IP) }, pointCnt = 0;//�������������㣬��������ѭ��
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
    //�������ͷ
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
    args_name = "";
    extargs_type = ST_EXTARGS_TYPE::null;
    extargs_num = 0;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����boolean
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_default��int_with_error
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_set_default��int_with_set_error
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str��ipaddr_with_default��ipaddr_with_error
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str_with_set_default��str_with_set_error
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
    if((extargs_type == ST_EXTARGS_TYPE::int_with_set_default || extargs_type == ST_EXTARGS_TYPE::int_with_set_error) && extargs_int_set != NULL)
        delete[] extargs_int_set;
    if ((extargs_type == ST_EXTARGS_TYPE::str_with_set_default || extargs_type == ST_EXTARGS_TYPE::str_with_set_error) && extargs_string_set != NULL)
        delete[] extargs_string_set;
}

/* ---------------------------------------------------------------
     ����AAT���Զ����Ա������ʵ�֣�private��
   ---------------------------------------------------------------- */

   /***************************************************************************
     �������ƣ�
     ��    �ܣ�
     ���������
     �� �� ֵ��
     ˵    ������ʵ�֣���Ҫ��
    ***************************************************************************/
const string args_analyse_tools::get_name() const
{
    return this->args_name;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������!!��ֻ����0/1
            ��ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
    return !!this->args_existed;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
    return this->extargs_int_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
    return this->extargs_string_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
    return this->extargs_ipaddr_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ extargs_ipaddr_value ��ֵ�� 0x7f000001 תΪ "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
    return to_string(((extargs_ipaddr_value) >> 3 * 8) & ((1 << 8) - 1)) + "." + to_string(((extargs_ipaddr_value) >> 2 * 8) & ((1 << 8) - 1)) + "." + to_string(((extargs_ipaddr_value) >> 1 * 8) & ((1 << 8) - 1)) + "." + to_string(((extargs_ipaddr_value) >> 0 * 8) & ((1 << 8) - 1)); //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������follow_up_args���Ƿ��к�������
            0  ���޺�������
            1  ���к�������
  �� �� ֵ��
  ˵    ������Ԫ����
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
                    cout << "����[--]��ʽ�Ƿ�(����--��ͷ����Ч����)." << endl;
                    return -1;
                }
                else {
                    cout << "����[" << argv[i] << "]�Ƿ�." << endl;
                    return -1;
                }
            }//��ʽ��ȷ��ƥ�䲻��
            if (args[index].args_existed) {
                cout << "����[" << argv[i] << "]�ظ�." << endl;
                return -1;
            }//�ظ�����
            args[index].args_existed = 1;
            if (args[index].extargs_num == 0)
                continue;
            else if (i == argc - 1 || isValidArgFormat(argv[i + 1])) {
                cout << "����[" << argv[i] << "]" << (i == argc - 1?"�ĸ��Ӳ�������":"ȱ�ٸ��Ӳ���") << ". (����:";
                switch (args[index].extargs_type)
                {
                    case ST_EXTARGS_TYPE::int_with_default:
                    case ST_EXTARGS_TYPE::int_with_error:
                        cout << "int, ��Χ[" << args[index].extargs_int_min << ".." << args[index].extargs_int_max << "]";
                        if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
                            ;
                        else
                            cout << " ȱʡ:" << args[index].extargs_int_default;
                        break;
                    case ST_EXTARGS_TYPE::int_with_set_default:
                    case ST_EXTARGS_TYPE::int_with_set_error:
                        cout << "int, ��Χ[";
                        for (const int* p = args[index].extargs_int_set;*p != INVALID_INT_VALUE_OF_SET;) {
                            cout << *p++;
                            cout << (*p == INVALID_INT_VALUE_OF_SET ? "]" : "/");
                        }
                        if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
                            ;
                        else
                            cout << " ȱʡ:" << args[index].extargs_int_default;
                        break;
                    case ST_EXTARGS_TYPE::str:
                        cout << "string";
                        if (!args[index].extargs_string_default.empty())
                            cout << " ȱʡ:" << args[index].extargs_string_default;
                        break;
                    case ST_EXTARGS_TYPE::ipaddr_with_default:
                    case ST_EXTARGS_TYPE::ipaddr_with_error:
                        cout << "IP��ַ";
                        if (args[index].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
                            ;
                        else
                            cout << " ȱʡ:" << args[index].get_str_ipaddr();
                        break;
                    case ST_EXTARGS_TYPE::str_with_set_default:
                    case ST_EXTARGS_TYPE::str_with_set_error:
                        cout << "string, ��ȡֵ[";
                        for (const string* p = args[index].extargs_string_set;!p->empty();) {
                            cout << *p++;
                            cout << (p->empty() ? "]" : "/");
                        }
                        if (args[index].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
                            ;
                        else
                            cout << " ȱʡ:" << args[index].extargs_string_default;
                }
                cout << ")" << endl;
                return -1;
            }//ȱ�ٸ��Ӳ���
            else {
                switch (args[index].extargs_type)
                {
                    case ST_EXTARGS_TYPE::int_with_default:
                    case ST_EXTARGS_TYPE::int_with_error:
                    {
                        if (!strIsInt(argv[i + 1])) {
                            cout << "����[" << argv[i] << "]�ĸ��Ӳ�����������. (����:int, ��Χ[" << args[index].extargs_int_min << ".." << args[index].extargs_int_max << "]";
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
                                cout << ")" << endl;
                            else
                                cout << " ȱʡ:" << args[index].extargs_int_default << ")" << endl;
                            return -1;
                        }
                        int value1 = atoi(argv[i + 1]);
                        if (value1 < args[index].extargs_int_min || value1 > args[index].extargs_int_max) {
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_error) {
                                cout << "����[" << argv[i] << "]�ĸ��Ӳ���ֵ(" << value1 << ")�Ƿ�. (����:int, ��Χ[" << args[index].extargs_int_min << ".." << args[index].extargs_int_max << "])" << endl;
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
                            cout << "����[" << argv[i] << "]�ĸ��Ӳ�����������. (����:int, ��Χ[";
                            for (const int* p = args[index].extargs_int_set;*p != INVALID_INT_VALUE_OF_SET;) {
                                cout << *p++;
                                cout << (*p == INVALID_INT_VALUE_OF_SET ? "]" : "/");
                            }
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
                                cout << ")" << endl;
                            else
                                cout << " ȱʡ:" << args[index].extargs_int_default << ")" << endl;
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
                                cout << "����[" << argv[i] << "]�ĸ��Ӳ���ֵ(" << value2 << ")�Ƿ�. (����:int, ��Χ[";
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
                            cout << "����[" << argv[i] << "]ȱ�ٸ��Ӳ���. (����:string";
                            if (!args[index].extargs_string_default.empty())
                                cout << " ȱʡ:" << args[index].extargs_string_default << ")" << endl;
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
                            cout << "����[" << argv[i] << "]�ĸ��Ӳ�������. (����:string, ��ȡֵ[";
                            for (const string* p = args[index].extargs_string_set;!p->empty();) {
                                cout << *p++;
                                cout << (p == args[index].extargs_string_set ? "]" : "/");
                            }
                            if (args[index].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
                                cout << ")" << endl;
                            else
                                cout << " ȱʡ:" << args[index].extargs_string_default << ")" << endl;
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
                                cout << "����[" << argv[i] << "]�ĸ��Ӳ���ֵ(" << argv[i + 1] << ")�Ƿ�. (����:string, ��ȡֵ[";
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
                                cout << "����[" << argv[i] << "]�ĸ��Ӳ���ֵ(" << argv[i + 1] << ")�Ƿ�. (����:IP��ַ)";
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
            cout << "����[" << argv[i] << "]��ʽ�Ƿ�(����--��ͷ����Ч����)." << endl;
            return -1;
        }
    }
    return cnt; //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
    int line =0;
    while (!args[line++].args_name.empty())//�и�ͷ�У�����ʹline�պô�1����
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
    return 0; //�˾������Ҫ�޸�
}

#endif // !ENABLE_LIB_COMMON_TOOLS
