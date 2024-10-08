/*2253731 �ƿ� �����*/
//���GB��ʽ��û���ã�������
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string.h>
#include <sstream>
#include "../include/class_aat.h"
#include "../include/class_crt.h"
#include "../include_mariadb_x86/mysql/mysql.h"
#include "hw_check.h"
#include "../include/linestring.h"

using namespace std;

const string status[] = { "��ȷ","����","δ�ύ","�ļ�С��3�ֽ�","Դ�ļ���ʽ����ȷ(��GB����)","PDF�ļ���ʽ����ȷ","����Ч����","���в���ע����","���в�������","���м�����","���в���ע����","�ļ���СΪ0","������������", "���ж���ע�͸�ʽ����ȷ","" };//"����"��ʵû�õ�

static void usage(const char* const full_procname)
{
    const char* procname = strchr(full_procname, '\\');
    if (procname == NULL)
        procname = full_procname;

    const int wkey = 7 + strlen(procname) + 1;
    const int wopt = 7 + strlen(procname) + 4;
    cout << endl;

    cout << "Usage: " << procname << " ��ѡ��/��ѡ��" << endl;
    cout << endl;
    cout << setw(wkey) << ' ' << "��ѡ�ָ������" << endl;
    cout << setw(wopt) << ' ' << "--action opname : ֧�ֵ�opname����" << endl;
    cout << setw(wopt) << ' ' << "\t base              : �������(�ļ��Ƿ��ύ���Ƿ񱻸Ķ��������Ƿ���ȷ)" << endl;
    cout << setw(wopt) << ' ' << "\t firstline         : ���м�飨��Դ�����ļ���Ҫ��" << endl;
    cout << setw(wopt) << ' ' << "\t secondline        : ���м�飨������Դ�����ļ���Ҫ��" << endl;

    cout << setw(wkey) << ' ' << "��ѡ�ָ���κ�" << endl;
    cout << setw(wopt) << ' ' << "--cno course_no     : �κ�" << endl;

    cout << setw(wkey) << ' ' << "��ѡ�ָ��ѧ��" << endl;
    cout << setw(wopt) << ' ' << "--stu no/all        : ָ������/ȫ��(7λ����Ϊѧ��,allΪȫ��)" << endl;

    cout << setw(wkey) << ' ' << "��ѡ�ָ���ļ�" << endl;
    cout << setw(wopt) << ' ' << "--file filename/all : �����ļ���/ȫ��(allΪȫ��,����Ϊ�����ļ���)" << endl;

    cout << setw(wkey) << ' ' << "��ѡ�" << endl;
    cout << setw(wopt) << ' ' << "--chapter n         : ��--file�Ļ������ٽ����½ڵ�ɸѡ(��/-1��ȫ���½�,����--week����)" << endl;
    cout << setw(wopt) << ' ' << "--week n            : ��--file�Ļ������ٽ����ܴε�ɸѡ(��/-1��ȫ���ܴ�,����--chapter����)" << endl;
    cout << setw(wopt) << ' ' << "--display xxxxx     : ÿλ0/1�ֱ��ʾ������Ϣ/δ�ύ��Ϣ/������Ϣ/������Ϣ/���ش�����Ϣ" << endl;
    cout << setw(wopt) << ' ' << "--cfgfile filename  : ָ�������ļ���(ȱʡ:hw_check.conf)" << endl;
    cout << endl;

    const char* DEMO_CNO = "10108001";
    const char* DEMO_SRC_FNAME = "12-b1.cpp";
    const char* DEMO_STUNO = "2259999";

    cout << "e.g.   " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all             : ���" << DEMO_CNO << "����ѧ����������ҵ�Ļ�����Ϣ" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : ���" << DEMO_CNO << "����ѧ���ĵ�4����ҵ�Ļ�����Ϣ" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --week 6    : ���" << DEMO_CNO << "����ѧ���ĵ�6����ҵ�Ļ�����Ϣ" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����������ҵ�Ļ�����Ϣ" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����" << DEMO_SRC_FNAME << "��ҵ�Ļ�����Ϣ" << endl;
    cout << endl;

    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all             : ���" << DEMO_CNO << "����ѧ����������ҵ��������Ϣ" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : ���" << DEMO_CNO << "����ѧ���ĵ�4����ҵ��������Ϣ" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --week 6    : ���" << DEMO_CNO << "����ѧ���ĵ�6����ҵ��������Ϣ" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����������ҵ��������Ϣ" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : ���" << DEMO_CNO << "��" << DEMO_STUNO << "ѧ����" << DEMO_SRC_FNAME << "��ҵ��������Ϣ" << endl;
    cout << endl;

    cout << "       " << procname << " --action secondline --cno " << DEMO_CNO << " --stu all --file " << DEMO_SRC_FNAME << " : ���" << DEMO_CNO << "������ѧ����" << DEMO_SRC_FNAME << "��ҵ�Ĵ�����Ϣ" << endl;
    cout << endl;

    cout << "       " << procname << " --cfgfile E:\\test\\my.conf --action base --cno " << DEMO_CNO << " --stu all --file all : ���" << DEMO_CNO << "����ѧ����������ҵ�Ļ�����Ϣ(ָ�������ļ�)" << endl;
    cout << endl;


    cout << endl;
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        usage(argv[0]);
        return 0;
    }
    const string actionType[] = { "base","firstline","secondline","" };
    const string cnoSet[] = { "10108001","5000244001601","" };
    args_analyse_tools args[] = {
        args_analyse_tools("--action",   ST_EXTARGS_TYPE::str_with_set_error, 1, -1, actionType),
        args_analyse_tools("--cno",      ST_EXTARGS_TYPE::str,                1, string("")),
        args_analyse_tools("--stu",      ST_EXTARGS_TYPE::str,                1, string("")),
        args_analyse_tools("--file",     ST_EXTARGS_TYPE::str,                1, string("")),
        args_analyse_tools("--chapter",  ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 99), //����-1��ʾ�������½�ѡ��
        args_analyse_tools("--week",     ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 20), //����-1��ʾ�������ܴ�ѡ��
        args_analyse_tools("--cfgfile",  ST_EXTARGS_TYPE::str,                1, string("hw_check.conf")),
        args_analyse_tools("--help",     ST_EXTARGS_TYPE::boolean,            0, false),
        args_analyse_tools("--debug",    ST_EXTARGS_TYPE::boolean,            0, false),
        args_analyse_tools("--display",  ST_EXTARGS_TYPE::str,                1, string("11111")),
        args_analyse_tools()  //���һ�������ڽ���
    };

    args_analyse_process(argc, argv, args, 0);

    if (args[7].existed()) {
        args_analyse_print(args);
        usage(argv[0]);
        return 0;
    }

    if(!args[0].existed()){
        usage(argv[0]);
        cout << "����[--action]����ָ��." << endl;
        return 0;
    }

    if(!args[1].existed()){
        usage(argv[0]);
        cout << "����[--cno]����ָ��." << endl;
        return 0;
    }

    if(!args[2].existed()){
        usage(argv[0]);
        cout << "����[--stu]����ָ��." << endl;
        return 0;
    }

    if(!args[3].existed()){
        usage(argv[0]);
        cout << "����[--file]����ָ��." << endl;
        return 0;
    }

    if ((args[4].existed() || args[5].existed()) && args[3].get_string() != "all") {
        usage(argv[0]);
        cout << "����[--chapter/--week]���ܳ�����[--file �����ļ���]ʱ." << endl;
        return 0;
    }

    if (args[2].get_string().length() != 7) {
        cout << "�ļ�[" << args[2].get_string() << "]�޷���." << endl;
        cout << endl;
        cout << "[--���ش���--] ��ȡ [--stu] ָ�����ļ�����." << endl;
    }

    if (args[2].get_string()!="all"&&args[0].get_string()=="secondline") {
        cout << "HW_Check_SecondLine ֻ�����ȫ��ѧ��" << endl;
        return 0;
    }
    
    cfgfile_read_tools cfg(args[6].get_string());

    MYSQL* mysql;
    MYSQL_RES* result;
    MYSQL_ROW  row;
    vector<MYSQL_ROW> rows;

    string stuNo = args[2].get_string();
    string fileName = args[3].get_string();
    string cno = args[1].get_string();
    string dbserver, dbuser, dbpasswd, dbname;
    cfg.item_get_value("[���ݿ�]", "db_host", "10.80.42.244", dbserver);
    cfg.item_get_value("[���ݿ�]", "db_name", "homework", dbname);
    cfg.item_get_value("[���ݿ�]", "db_username", "hwcheck", dbuser);
    cfg.item_get_value("[���ݿ�]", "db_passwd", "hw_CheCk-For23241*oOP", dbpasswd);

    /* ��ʼ�� mysql ������ʧ�ܷ���NULL */
    if ((mysql = mysql_init(NULL)) == NULL) {
        cout << "mysql_init failed" << endl;
        return -1;
    }

    /* �������ݿ⣬ʧ�ܷ���NULL
        1��mysqldû����
        2��û��ָ�����Ƶ����ݿ���� */
    if (mysql_real_connect(mysql, dbserver.c_str(), dbuser.c_str(), dbpasswd.c_str(), dbname.c_str(), 0, NULL, 0) == NULL
        ) {
        cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* �����ַ���������������ַ����� */
    mysql_set_character_set(mysql, "gbk");

    string rootDir;
    cfg.item_get_value("[�ļ�Ŀ¼]", "src_rootdir", "D:\\��ҵ\\OOP\\23241-source\\", rootDir);

    string searchName;
    if (stuNo != "all") {
        searchName = string("SELECT * FROM `view_hwcheck_stulist` WHERE `stu_no` = ") + stuNo + " AND `stu_cno` = " + cno;
    }
    else {
        searchName = string("SELECT * FROM `view_hwcheck_stulist` WHERE `stu_cno` = ") + cno;
    }
    if (mysql_query(mysql, searchName.c_str())) {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }
    if ((result = mysql_store_result(mysql)) == NULL) {
        cout << "mysql_store_result failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }
    while ((row = mysql_fetch_row(result)) != NULL)
        rows.push_back(row);
    if(rows.size()==0){
        cout << "���β�����ѧ������Ϊ0/�ļ�����Ϊ0��δִ��" << endl;
        return 0;
    }
    if (args[3].get_string() != "all") {//�����ļ�
        unsigned stuCnt = 1;
        int statusCnt[STATUS_NUM] = { 0 };
        vector<VeriList> veriList(rows.size());
        cout << "�κ� : " << cno << " ѧ������ : " << rows.size() << " Դ�ļ��� : " << fileName << endl;
        for (stuCnt = 0;stuCnt < rows.size();stuCnt++) {
            string stuName = rows[stuCnt][3];
            stuNo = rows[stuCnt][2];
            veriList[stuCnt].stuNo = stuNo;
            veriList[stuCnt].stuName = stuName;
            cout << left << setw(3) << stuCnt + 1 << ": " << stuNo << '/' << setw(10) << stuName << ": ";
            
            switch (checkFileBase(rootDir + cno + '-' + stuNo + '\\' + fileName)) {
                case FILE_LESS_THAN_3B:
                    cout << status[FILE_LESS_THAN_3B] << endl;
                    statusCnt[FILE_LESS_THAN_3B]++;
                    break;
                case FILE_EMPTY:
                    cout << status[FILE_EMPTY] << endl;
                    statusCnt[FILE_EMPTY]++;
                    break;

                case CORRECT:
                    if (args[0].get_string() == "base") {
                        cout << status[CORRECT] << endl;
                        statusCnt[CORRECT]++;
                        break;
                    }
                    else if (args[0].get_string() == "firstline") {
                        ifstream fin(rootDir + cno + '-' + stuNo + '\\' + fileName);
                        string firstline;
                        while (getline(fin, firstline)) {
                            trimLine(firstline, "all");
                            if(!firstline.empty()){
                                break;
                            }
                        }
                        if (firstline.empty()) {
                            cout << status[NO_VALID_FIRSTLINE] << endl;
                            statusCnt[NO_VALID_FIRSTLINE]++;
                        }
                        else if (judgeCcomment(firstline) == false) {
                            cout << status[FIRSTLINE_NOT_COMMENT] << '[' << firstline << ']' << endl;
                            statusCnt[FIRSTLINE_NOT_COMMENT]++;
                        }
                        else {
                            switch(checkFileFirstLine(firstline, rows[stuCnt], stuNo, stuName)) {
                                case FIRSTLINE_NOT_3_ITEMS:
                                    statusCnt[FIRSTLINE_NOT_3_ITEMS]++;
                                    break;
                                case FIRSTLINE_ERROR:
                                    statusCnt[FIRSTLINE_ERROR]++;
                                    break;
                                case CORRECT:
                                    statusCnt[CORRECT]++;
                                    break;
                            }
                        }
                    }
                    else {
                        ifstream fin(rootDir + cno + '-' + stuNo + '\\' + fileName);
                        string secondline;
                        while (getline(fin, secondline)) {
                            trimLine(secondline, "all");
                            if (!secondline.empty()) {
                                break;
                            }
                        }
                        while(getline(fin, secondline)){
                            trimLine(secondline, "all");
                            if (!secondline.empty()) {
                                break;
                            }
                        }
                        if (secondline.empty()) {
                            cout<<status[SECONDLINE_OTHER_ERROR]<<endl;
                            statusCnt[SECONDLINE_OTHER_ERROR]++;
                        }
                        else {
                            trimLine(secondline, "all");
                            if (judgeCcomment(secondline) == false) {
                                cout<<status[SECONDLINE_NOT_COMMENT]<<endl;
                                statusCnt[SECONDLINE_NOT_COMMENT]++;
                            }
                            else {
                                string validParts;
                                if (secondline.substr(0, 2) == "//") {
                                    validParts = secondline.substr(2);
                                }
                                else {
                                    validParts = secondline.substr(2, secondline.length() - 4);
                                }
                                stringstream ss(validParts);
                                vector<string> parts;
                                bool flag = true;
                                while (ss >> validParts) {
                                    parts.push_back(validParts);
                                }
                                unsigned int i;
                                for (i = 0; i < parts.size(); i += 2) {
                                    if (parts[i] == stuNo) {
                                        cout << "��[" << i/2+1 << "]��д���Լ�" << endl;
                                        flag = false;
                                        break;
                                    }
                                    if (parts[i].length() != 7) {
                                        cout << "��" << i/2+1 << "λͬѧ��ѧ��[" << parts[i] << "]����7λ���������ݺ���" << endl;
                                        flag = false;
                                        break;
                                    }
                                    if (i + 1 == parts.size()) {
                                        cout << "��[" << i/2+1 << "]��ѧ���������Ϣ��ȫ(ֻ����һ��)���������ݺ���" << endl;
                                        flag = false;
                                        break;
                                    }
                                }
                                if (flag) {
                                    cout << "��ȷ" << endl;
                                }
                                statusCnt[CORRECT]++;
                                for (unsigned int j = 0; j < i; j++) {
                                    veriList[stuCnt].mates.push_back(parts[j]);
                                }//���ɻ�������
                            }
                        }
                    }
                    break;
                case NO_SUBMIT:
                    statusCnt[NO_SUBMIT]++;
                    cout << status[NO_SUBMIT] << endl;
                    break;
                    break;
                case NOT_GB:
                    statusCnt[NOT_GB]++;
                    cout << status[NOT_GB] << endl;
                    break;
                case NOT_PDF:
                    statusCnt[NOT_PDF]++;
                    cout << status[NOT_PDF] << endl;
                    break;
            }
        }
        cout << "ȫ��ͨ��" << statusCnt[CORRECT] << '/' << stuCnt << "��ѧ��������ͨ��" << statusCnt[CORRECT] << "��" << endl;
        cout << "================" << endl;
        cout << "��ϸ��Ϣ" << endl;
        cout << "================" << endl;
        int length = 0;
        for (int i = 0;i < STATUS_NUM;i++) {
            if (statusCnt[i])
                length = max(length, (int)status[i].length());
        }
        for (int i = 0;i < STATUS_NUM;i++) {
            if (statusCnt[i]) {
                cout << "  " << right << setw(length) << status[i] << " : " << statusCnt[i] << endl;
            }
        }
        cout << "================" << endl;
        cout << endl;
        if (args[0].get_string() == "secondline") {
            cout << "����������" << endl;
            for (stuCnt = 0;stuCnt < rows.size();stuCnt++) {
                cout << left << setw(3) << stuCnt + 1 << ": " << veriList[stuCnt].stuNo << ' ' << veriList[stuCnt].stuName << endl;
                for (unsigned int i = 0;i < veriList[stuCnt].mates.size();i += 2) {
                    cout << '\t' << veriList[stuCnt].mates[i] << ' ' << veriList[stuCnt].mates[i + 1];
                    int index;
                    if ((index = existInVector(veriList, VeriList(veriList[stuCnt].mates[i]))) == -1) {
                        cout << '\t' << "�Է�ѧ�Ų�����";
                    }
                    else if (veriList[index].stuName != veriList[stuCnt].mates[i + 1]) {
                        cout << '\t' << "�Է���������ȷ";
                    }
                    else {
                        int index2;
                        if ((index2 = existInVector(veriList[index].mates, veriList[stuCnt].stuNo)) == -1) {
                            cout << '\t' << "��������";
                        }
                        else if (veriList[index].mates[index2 + 1] != veriList[stuCnt].stuName) {
                            cout << '\t' << "ûд��������";
                        }
                    }

                    cout << endl;
                }
            }
        }
    }
    else {//ȫ���ļ�
        vector<string> hwRows;
        searchName = string("SELECT * FROM `view_hwcheck_hwlist` WHERE `hw_cno` = ") + cno;
        if(args[4].get_int()!=-1){
            searchName += " AND `hw_chapter` = " + to_string(args[4].get_int());
        }
        if(args[5].get_int()!=-1){
            searchName += " AND `hw_week` = " + to_string(args[5].get_int());
        }
        if (mysql_query(mysql, searchName.c_str())) {
            cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
            return -1;
        }
        if ((result = mysql_store_result(mysql)) == NULL) {
            cout << "mysql_store_result failed(" << mysql_error(mysql) << ")" << endl;
            return -1;
        }
        while ((row = mysql_fetch_row(result)) != NULL)
            hwRows.push_back(row[5]);
        if (hwRows.size() == 0) {
            cout << "���β�����ѧ������Ϊ0/�ļ�����Ϊ0��δִ��" << endl;
            return 0;
        }
        int hw_length = 0;
        for(unsigned int i=0;i<hwRows.size();i++){
            hw_length = max(hw_length, (int)hwRows[i].length());
        }
        int totalStatusCnt[STATUS_NUM] = { 0 };
        unsigned int stuCnt = 0;
        for (stuCnt = 0;stuCnt < rows.size();stuCnt++) {
            stuNo = rows[stuCnt][2];
            string stuName = rows[stuCnt][3];
            int statusCnt[STATUS_NUM] = { 0 };
            cout << stuCnt + 1 << " : ѧ��-" << stuNo << " ����-" << stuName << " �κ�-" << cno << " �ļ�����-" << hwRows.size() << endl;
            for (unsigned int i = 0;i < hwRows.size();i++) {
                string hwName = hwRows[i];
                string fex = hwName.substr(hwName.find_last_of('.'));
                if (args[0].get_string() != "base" && fex != ".cpp" && fex != ".h" && fex != ".hpp" && fex != ".c") {
                    continue;
                }
                cout << "  " << left << setw(hw_length) << hwName << " : ";
                switch (checkFileBase(rootDir + cno + '-' + stuNo + '\\' + hwName)) {
                    case FILE_LESS_THAN_3B:
                        cout << status[FILE_LESS_THAN_3B] << endl;
                        statusCnt[FILE_LESS_THAN_3B]++;
                        break;
                    case CORRECT:
                        if (args[0].get_string() == "base") {
                            cout << "��ȷ" << endl;
                            statusCnt[CORRECT]++;
                            break;
                        }
                        else {
                            ifstream fin(rootDir + cno + '-' + stuNo + '\\' + hwName);
                            string firstline;
                            getline(fin, firstline);
                            int firstline_status = judgeCcomment(firstline);
                            if (firstline_status == 0) {
                                cout << status[FIRSTLINE_NOT_COMMENT] << " [" << firstline << ']' << endl;
                                statusCnt[FIRSTLINE_NOT_COMMENT]++;
                            }
                            else if (firstline_status==2) {
                                cout << "���в��Ƿ���Ҫ���/* */��ʽ [" << firstline << ']' << endl;
                                statusCnt[FIRSTLINE_CMT_IN_DIFF_LINE]++;
                            }
                            else {
                                switch (checkFileFirstLine(firstline, rows[stuCnt], stuNo, stuName)) {
                                    case FIRSTLINE_NOT_3_ITEMS:
                                        statusCnt[FIRSTLINE_NOT_3_ITEMS]++;
                                        break;
                                    case FIRSTLINE_ERROR:
                                        statusCnt[FIRSTLINE_ERROR]++;
                                        break;
                                    case CORRECT:
                                        statusCnt[CORRECT]++;
                                        break;
                                }
                            }
                        }
                        break;
                    case NO_SUBMIT:
                        statusCnt[NO_SUBMIT]++;
                        cout << status[NO_SUBMIT] << endl;
                        break;
                    case NOT_GB:
                        statusCnt[NOT_GB]++;
                        cout << status[NOT_GB] << endl;
                        break;
                    case NOT_PDF:
                        statusCnt[NOT_PDF]++;
                        cout << status[NOT_PDF] << endl;
                        break;
                }
            }
            int length = 0;
            for (int i = 0; i < STATUS_NUM; i++) {
                if (statusCnt[i])
                    length = max(length, (int)status[i].length());
            }
            cout << "���ͨ��" << statusCnt[CORRECT] << '/' << hwRows.size() << "���ļ�������ͨ��" << statusCnt[CORRECT] << "��" << endl;
            cout << setw(length+12)<<setfill('-')<<""<<setfill(' ') << endl;
            cout << "ѧ����ϸ��Ϣ" << endl;
            cout << setw(length + 12) << setfill('-') << "" << setfill(' ') << endl;
            for (int i = 0; i < STATUS_NUM; i++) {
                if (statusCnt[i]) {
                    cout << "  " << right << setw(length) << status[i] << " : " << statusCnt[i] << endl;
                }
            }
            cout << setw(length + 12) << setfill('-') << "" << setfill(' ') << endl;
            for(int i=0;i<STATUS_NUM;i++){
                totalStatusCnt[i] += statusCnt[i];
            }
            cout << endl;
        }
        cout << endl;
        int length = 0;
        for (int i = 0;i < STATUS_NUM;i++) {
            if (totalStatusCnt[i])
                length = max(length, (int)status[i].length());
        }
        int hwSum = 0;
        for (int i = 0;i < STATUS_NUM;i++) {
            hwSum += totalStatusCnt[i]; 
        }
        cout << "�����" << stuCnt << "��ѧ���ļ�飬�ļ�����:" << hwSum << "��ͨ������:" << totalStatusCnt[CORRECT] << "������ͨ��" << totalStatusCnt[CORRECT] << "��" << endl;
        cout<<setw(length+12)<<setfill('=')<<""<<setfill(' ')<<endl;
        cout << "������ϸ��Ϣ" << endl;
        cout<<setw(length+12)<<setfill('=')<<""<<setfill(' ')<<endl;
        for (int i = 0;i < STATUS_NUM;i++) {
            if (totalStatusCnt[i]) {
                cout << "  " << right << setw(length) << status[i] << " : " << totalStatusCnt[i] << endl;
            }
        }
        cout<<setw(length+12)<<setfill('=')<<""<<setfill(' ')<<endl;
    }

}
