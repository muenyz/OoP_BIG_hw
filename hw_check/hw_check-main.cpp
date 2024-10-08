/*2253731 计科 张皓钦*/
//检查GB格式的没做好，搞乱了
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

const string status[] = { "正确","错误","未提交","文件小于3字节","源文件格式不正确(非GB编码)","PDF文件格式不正确","无有效首行","首行不是注释行","首行不是三项","首行检查出错","次行不是注释行","文件大小为0","次行其它错误", "首行多行注释格式不正确","" };//"错误"其实没用到

static void usage(const char* const full_procname)
{
    const char* procname = strchr(full_procname, '\\');
    if (procname == NULL)
        procname = full_procname;

    const int wkey = 7 + strlen(procname) + 1;
    const int wopt = 7 + strlen(procname) + 4;
    cout << endl;

    cout << "Usage: " << procname << " 必选项/可选项" << endl;
    cout << endl;
    cout << setw(wkey) << ' ' << "必选项：指定操作" << endl;
    cout << setw(wopt) << ' ' << "--action opname : 支持的opname如下" << endl;
    cout << setw(wopt) << ' ' << "\t base              : 基础检查(文件是否提交、是否被改动、编码是否正确)" << endl;
    cout << setw(wopt) << ' ' << "\t firstline         : 首行检查（仅源程序文件需要）" << endl;
    cout << setw(wopt) << ' ' << "\t secondline        : 次行检查（仅部分源程序文件需要）" << endl;

    cout << setw(wkey) << ' ' << "必选项：指定课号" << endl;
    cout << setw(wopt) << ' ' << "--cno course_no     : 课号" << endl;

    cout << setw(wkey) << ' ' << "必选项：指定学生" << endl;
    cout << setw(wopt) << ' ' << "--stu no/all        : 指定单个/全部(7位数字为学号,all为全部)" << endl;

    cout << setw(wkey) << ' ' << "必选项：指定文件" << endl;
    cout << setw(wopt) << ' ' << "--file filename/all : 具体文件名/全部(all为全部,其余为具体文件名)" << endl;

    cout << setw(wkey) << ' ' << "可选项：" << endl;
    cout << setw(wopt) << ' ' << "--chapter n         : 在--file的基础上再进行章节的筛选(无/-1则全部章节,可与--week共存)" << endl;
    cout << setw(wopt) << ' ' << "--week n            : 在--file的基础上再进行周次的筛选(无/-1则全部周次,可与--chapter共存)" << endl;
    cout << setw(wopt) << ' ' << "--display xxxxx     : 每位0/1分别表示正常信息/未提交信息/错误信息/汇总信息/严重错误信息" << endl;
    cout << setw(wopt) << ' ' << "--cfgfile filename  : 指定配置文件名(缺省:hw_check.conf)" << endl;
    cout << endl;

    const char* DEMO_CNO = "10108001";
    const char* DEMO_SRC_FNAME = "12-b1.cpp";
    const char* DEMO_STUNO = "2259999";

    cout << "e.g.   " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all             : 检查" << DEMO_CNO << "所有学生的所有作业的基本信息" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : 检查" << DEMO_CNO << "所有学生的第4章作业的基本信息" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --week 6    : 检查" << DEMO_CNO << "所有学生的第6周作业的基本信息" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的所有作业的基本信息" << endl;
    cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的" << DEMO_SRC_FNAME << "作业的基本信息" << endl;
    cout << endl;

    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all             : 检查" << DEMO_CNO << "所有学生的所有作业的首行信息" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : 检查" << DEMO_CNO << "所有学生的第4章作业的首行信息" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --week 6    : 检查" << DEMO_CNO << "所有学生的第6周作业的首行信息" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的所有作业的首行信息" << endl;
    cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的" << DEMO_SRC_FNAME << "作业的首行信息" << endl;
    cout << endl;

    cout << "       " << procname << " --action secondline --cno " << DEMO_CNO << " --stu all --file " << DEMO_SRC_FNAME << " : 检查" << DEMO_CNO << "的所有学生的" << DEMO_SRC_FNAME << "作业的次行信息" << endl;
    cout << endl;

    cout << "       " << procname << " --cfgfile E:\\test\\my.conf --action base --cno " << DEMO_CNO << " --stu all --file all : 检查" << DEMO_CNO << "所有学生的所有作业的基本信息(指定配置文件)" << endl;
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
        args_analyse_tools("--chapter",  ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 99), //参数-1表示不进行章节选择
        args_analyse_tools("--week",     ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 20), //参数-1表示不进行周次选择
        args_analyse_tools("--cfgfile",  ST_EXTARGS_TYPE::str,                1, string("hw_check.conf")),
        args_analyse_tools("--help",     ST_EXTARGS_TYPE::boolean,            0, false),
        args_analyse_tools("--debug",    ST_EXTARGS_TYPE::boolean,            0, false),
        args_analyse_tools("--display",  ST_EXTARGS_TYPE::str,                1, string("11111")),
        args_analyse_tools()  //最后一个，用于结束
    };

    args_analyse_process(argc, argv, args, 0);

    if (args[7].existed()) {
        args_analyse_print(args);
        usage(argv[0]);
        return 0;
    }

    if(!args[0].existed()){
        usage(argv[0]);
        cout << "参数[--action]必须指定." << endl;
        return 0;
    }

    if(!args[1].existed()){
        usage(argv[0]);
        cout << "参数[--cno]必须指定." << endl;
        return 0;
    }

    if(!args[2].existed()){
        usage(argv[0]);
        cout << "参数[--stu]必须指定." << endl;
        return 0;
    }

    if(!args[3].existed()){
        usage(argv[0]);
        cout << "参数[--file]必须指定." << endl;
        return 0;
    }

    if ((args[4].existed() || args[5].existed()) && args[3].get_string() != "all") {
        usage(argv[0]);
        cout << "参数[--chapter/--week]不能出现在[--file 单个文件名]时." << endl;
        return 0;
    }

    if (args[2].get_string().length() != 7) {
        cout << "文件[" << args[2].get_string() << "]无法打开." << endl;
        cout << endl;
        cout << "[--严重错误--] 读取 [--stu] 指定的文件出错." << endl;
    }

    if (args[2].get_string()!="all"&&args[0].get_string()=="secondline") {
        cout << "HW_Check_SecondLine 只能针对全体学生" << endl;
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
    cfg.item_get_value("[数据库]", "db_host", "10.80.42.244", dbserver);
    cfg.item_get_value("[数据库]", "db_name", "homework", dbname);
    cfg.item_get_value("[数据库]", "db_username", "hwcheck", dbuser);
    cfg.item_get_value("[数据库]", "db_passwd", "hw_CheCk-For23241*oOP", dbpasswd);

    /* 初始化 mysql 变量，失败返回NULL */
    if ((mysql = mysql_init(NULL)) == NULL) {
        cout << "mysql_init failed" << endl;
        return -1;
    }

    /* 连接数据库，失败返回NULL
        1、mysqld没运行
        2、没有指定名称的数据库存在 */
    if (mysql_real_connect(mysql, dbserver.c_str(), dbuser.c_str(), dbpasswd.c_str(), dbname.c_str(), 0, NULL, 0) == NULL
        ) {
        cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* 设置字符集，否则读出的字符乱码 */
    mysql_set_character_set(mysql, "gbk");

    string rootDir;
    cfg.item_get_value("[文件目录]", "src_rootdir", "D:\\作业\\OOP\\23241-source\\", rootDir);

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
        cout << "本次操作的学生数量为0/文件数量为0，未执行" << endl;
        return 0;
    }
    if (args[3].get_string() != "all") {//单个文件
        unsigned stuCnt = 1;
        int statusCnt[STATUS_NUM] = { 0 };
        vector<VeriList> veriList(rows.size());
        cout << "课号 : " << cno << " 学生数量 : " << rows.size() << " 源文件名 : " << fileName << endl;
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
                                        cout << "第[" << i/2+1 << "]项写了自己" << endl;
                                        flag = false;
                                        break;
                                    }
                                    if (parts[i].length() != 7) {
                                        cout << "第" << i/2+1 << "位同学的学号[" << parts[i] << "]不是7位，后续内容忽略" << endl;
                                        flag = false;
                                        break;
                                    }
                                    if (i + 1 == parts.size()) {
                                        cout << "第[" << i/2+1 << "]个学生后面的信息不全(只读到一项)，后续内容忽略" << endl;
                                        flag = false;
                                        break;
                                    }
                                }
                                if (flag) {
                                    cout << "正确" << endl;
                                }
                                statusCnt[CORRECT]++;
                                for (unsigned int j = 0; j < i; j++) {
                                    veriList[stuCnt].mates.push_back(parts[j]);
                                }//生成互验名单
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
        cout << "全部通过" << statusCnt[CORRECT] << '/' << stuCnt << "个学生，本次通过" << statusCnt[CORRECT] << "个" << endl;
        cout << "================" << endl;
        cout << "详细信息" << endl;
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
            cout << "交叉检查结果：" << endl;
            for (stuCnt = 0;stuCnt < rows.size();stuCnt++) {
                cout << left << setw(3) << stuCnt + 1 << ": " << veriList[stuCnt].stuNo << ' ' << veriList[stuCnt].stuName << endl;
                for (unsigned int i = 0;i < veriList[stuCnt].mates.size();i += 2) {
                    cout << '\t' << veriList[stuCnt].mates[i] << ' ' << veriList[stuCnt].mates[i + 1];
                    int index;
                    if ((index = existInVector(veriList, VeriList(veriList[stuCnt].mates[i]))) == -1) {
                        cout << '\t' << "对方学号不存在";
                    }
                    else if (veriList[index].stuName != veriList[stuCnt].mates[i + 1]) {
                        cout << '\t' << "对方姓名不正确";
                    }
                    else {
                        int index2;
                        if ((index2 = existInVector(veriList[index].mates, veriList[stuCnt].stuNo)) == -1) {
                            cout << '\t' << "抛弃了你";
                        }
                        else if (veriList[index].mates[index2 + 1] != veriList[stuCnt].stuName) {
                            cout << '\t' << "没写对你名字";
                        }
                    }

                    cout << endl;
                }
            }
        }
    }
    else {//全部文件
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
            cout << "本次操作的学生数量为0/文件数量为0，未执行" << endl;
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
            cout << stuCnt + 1 << " : 学号-" << stuNo << " 姓名-" << stuName << " 课号-" << cno << " 文件数量-" << hwRows.size() << endl;
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
                            cout << "正确" << endl;
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
                                cout << "首行不是符合要求的/* */格式 [" << firstline << ']' << endl;
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
            cout << "检查通过" << statusCnt[CORRECT] << '/' << hwRows.size() << "个文件，本次通过" << statusCnt[CORRECT] << "个" << endl;
            cout << setw(length+12)<<setfill('-')<<""<<setfill(' ') << endl;
            cout << "学生详细信息" << endl;
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
        cout << "共完成" << stuCnt << "个学生的检查，文件总数:" << hwSum << "，通过总数:" << totalStatusCnt[CORRECT] << "，本次通过" << totalStatusCnt[CORRECT] << "个" << endl;
        cout<<setw(length+12)<<setfill('=')<<""<<setfill(' ')<<endl;
        cout << "整体详细信息" << endl;
        cout<<setw(length+12)<<setfill('=')<<""<<setfill(' ')<<endl;
        for (int i = 0;i < STATUS_NUM;i++) {
            if (totalStatusCnt[i]) {
                cout << "  " << right << setw(length) << status[i] << " : " << totalStatusCnt[i] << endl;
            }
        }
        cout<<setw(length+12)<<setfill('=')<<""<<setfill(' ')<<endl;
    }

}
