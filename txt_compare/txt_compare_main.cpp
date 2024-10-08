/* 2253731 计科 张皓钦 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//按需加入需要的头文件 
#include <fstream>
#include <iomanip>
#include <string.h>
#include <sstream>
#include "../include/class_aat.h"
#include "../include/cmd_console_tools.h"
#include "../include/linestring.h"
using namespace std;

enum OPT_ARGS {
	OPT_ARGS_HELP = 0,
	OPT_ARGS_DEBUG,
	OPT_ARGS_FILE1,
	OPT_ARGS_FILE2,
	OPT_ARGS_TRIM,
	OPT_ARGS_LINE_SKIP,
	OPT_ARGS_LINE_OFFSET,
	OPT_ARGS_IGNORE_BLANK,
	OPT_ARGS_CR_CRLF_NOT_EQUAL,
	OPT_ARGS_MAX_DIFFNUM,
	OPT_ARGS_MAX_LINENUM,
	OPT_ARGS_DISPLAY
};

static void printRuler(int length,const int preblank=0)
{
	length = (length/10+1)*10;
	if(length==0)
		length = 10;
	cout << setfill(' ') << setw(preblank) << "";
	cout << setfill('-') << setw(length) << "" << endl;
	cout << setfill(' ') << setw(preblank) << "";
	cout << setfill(' ') << left;
	for(int i=0;i<length;i+=10)
		cout << setw(10) << i/10%10;
	cout << right << endl;
	cout << setfill(' ') << setw(preblank) << "";
	for (int i = 0;i < length;i += 10)
		for(int j=0;j<10;j++)
			cout << j;
	cout << endl;
	cout << setfill(' ') << setw(preblank) << "";
	cout << setfill('-') << setw(length) << "" << setfill(' ') << endl;
}

static void getProcname(char* procname, const char* fullPath)
{
	const char* p = strrchr(fullPath, '\\');
	if (p == NULL)
		p = fullPath;
	else
		p++;
	strcpy(procname, p);
}





//从1开始记数
static unsigned int misMatch(const string& line1, const string& line2)
{
	unsigned int i = 0;
	for (; i < line1.length() && i < line2.length(); i++)
	{
		if (line1[i] != line2[i])
			break;
	}
	i= (i == line1.length() && i == line2.length()) ? 0 : i + 1;
	return i;
}

static void printDiffLine(const string& lineToPrint, const string& lineToCompare)
{
	unsigned int i = 0;
	char ch;
	bool cnc = false;
	if(lineToPrint.length()==0)
	{
		cct_setcolor(COLOR_HYELLOW, COLOR_RED);
		cout << "<EMPTY>";
		cct_setcolor();
		return;
	}
	for (;i < lineToPrint.length() && i < lineToCompare.length();i++) {
		if (lineToPrint[i] != lineToCompare[i])
			cct_setcolor(COLOR_HYELLOW, COLOR_RED);
		ch = lineToPrint[i];
		if(ch=='\r'||ch=='\n'||ch=='\v'||ch=='\b'||ch=='\a')
			ch = 'X';
		if (cnc) {
				cout << lineToPrint[i - 1];
				cnc = false;
				cout << ch;
		}
		else if (ch < 0)
			cnc = true;
		else
			cout << ch;
		cct_setcolor();
	}
	cct_setcolor(COLOR_HYELLOW, COLOR_RED);
	for (;i < lineToPrint.length();i++) {
		ch = lineToPrint[i];
		if (ch == '\r' || ch == '\n' || ch == '\v' || ch == '\b' || ch == '\a')
			ch = 'X';
		if (cnc) {
			cout << lineToPrint[i - 1];
			cnc = false;
			cout << ch;
		}
		else if (ch < 0)
			cnc = true;
		else
			cout << ch;
	}
	cct_setcolor();
}

static int compDiff(args_analyse_tools *args)
{
	ifstream file1;
	ifstream file2;
	if (args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed()) {
		file1.open(args[OPT_ARGS_FILE1].get_string().c_str(), ios::in | ios::binary);
		file2.open(args[OPT_ARGS_FILE2].get_string().c_str(), ios::in | ios::binary);
	}
	else {
		file1.open(args[OPT_ARGS_FILE1].get_string().c_str(), ios::in);
		file2.open(args[OPT_ARGS_FILE2].get_string().c_str(), ios::in);
	}
	if (!file1||!file2)
		return -1;
	string line1, line2;
	unsigned int maxLen = 0;
	while (!file1.eof()) {
		getline(file1, line1);
		maxLen = maxLen > line1.length() ? maxLen : line1.length();
	}
	while (!file2.eof()) {
		getline(file2, line2);
		maxLen = maxLen > line2.length() ? maxLen : line2.length();
	}
	file1.clear();
	file2.clear();
	file1.seekg(0, ios::beg);
	file2.seekg(0, ios::beg);
	//分隔行===的宽度计算 
	int width = (maxLen / 10 + 1) * 10 + 8 + 2; //比---的标尺多2个：line_maxlen向上取10倍整数，8是"文件1 : "

	/* 如果加了hex输出，则最小宽度为80 */
	if (args[OPT_ARGS_DISPLAY].existed() && args[OPT_ARGS_DISPLAY].get_string()=="detailed" && width < 80)
		width = 80;
	if (args[OPT_ARGS_DISPLAY].existed() && (args[OPT_ARGS_DISPLAY].get_string() == "normal" || args[OPT_ARGS_DISPLAY].get_string() == "detailed")){
		cout << "比较结果输出：" << endl;
		cout << setfill('=') << setw(width) << "" << setfill(' ') << endl;
	}
	int line_num1 = 0, line_num2 = 0, diffCnt = 0, cmpCnt = 0;
	unsigned diffLoc;
	bool flag = true;
	if(args[OPT_ARGS_LINE_SKIP].existed())
	{
		int skip = args[OPT_ARGS_LINE_SKIP].get_int();
		while (skip--)
		{
			getline(file1, line1);
			getline(file2, line2);
			if(args[OPT_ARGS_TRIM].existed())
			{
				trimLine(line1, args[OPT_ARGS_TRIM].get_string());
				trimLine(line2, args[OPT_ARGS_TRIM].get_string());
			}
			if(args[OPT_ARGS_IGNORE_BLANK].existed())
			{
				while(line1.empty()&&!file1.eof())
				{
					line_num1++;
					getline(file1, line1);
					if(args[OPT_ARGS_TRIM].existed())
						trimLine(line1, args[OPT_ARGS_TRIM].get_string());
				}
				while(line2.empty()&&!file2.eof())
				{
					line_num2++;
					getline(file2, line2);
					if(args[OPT_ARGS_TRIM].existed())
						trimLine(line2, args[OPT_ARGS_TRIM].get_string());
				}
			}
			line_num1++;
			line_num2++;
		}
	}
	if(args[OPT_ARGS_LINE_OFFSET].existed())
	{
		int offset = args[OPT_ARGS_LINE_OFFSET].get_int();
		if(offset<0)
		{
			while (offset++)
			{
				getline(file1, line1);
				line_num1++;
				if(args[OPT_ARGS_TRIM].existed())
					trimLine(line1, args[OPT_ARGS_TRIM].get_string());
				if (args[OPT_ARGS_IGNORE_BLANK].existed()){
					while(line1.empty()&&!file1.eof())
					{
						line_num1++;
						getline(file1, line1);
						if(args[OPT_ARGS_TRIM].existed())
							trimLine(line1, args[OPT_ARGS_TRIM].get_string());
					}
				}
			}
		}
		else
		{
			while (offset--)
			{
				getline(file2, line2);
				line_num2++;
				if (args[OPT_ARGS_TRIM].existed())
					trimLine(line2, args[OPT_ARGS_TRIM].get_string());
				if (args[OPT_ARGS_IGNORE_BLANK].existed()) {
					while (line2.empty() && !file2.eof())
					{
						line_num2++;
						getline(file2, line2);
						if (args[OPT_ARGS_TRIM].existed())
							trimLine(line2, args[OPT_ARGS_TRIM].get_string());
					}
				}
			}
		}
	}
	while (++line_num1&&++line_num2)
	{
		if(file1.eof()||file2.eof())
			break;
		if (args[OPT_ARGS_MAX_DIFFNUM].existed() && diffCnt >= args[OPT_ARGS_MAX_DIFFNUM].get_int())
			break;
		if (args[OPT_ARGS_MAX_LINENUM].existed() && cmpCnt >= args[OPT_ARGS_MAX_LINENUM].get_int())
			break;
		getline(file1, line1);
		getline(file2, line2);
		if(args[OPT_ARGS_TRIM].existed())
		{
			trimLine(line1, args[OPT_ARGS_TRIM].get_string());
			trimLine(line2, args[OPT_ARGS_TRIM].get_string());
		}
		if(args[OPT_ARGS_IGNORE_BLANK].existed())
		{
			while(line1.empty()&&!file1.eof())
			{
				line_num1++;
				getline(file1, line1);
				if(args[OPT_ARGS_TRIM].existed())
					trimLine(line1, args[OPT_ARGS_TRIM].get_string());
			}
			while(line2.empty()&&!file2.eof())
			{
				line_num2++;
				getline(file2, line2);
				if(args[OPT_ARGS_TRIM].existed())
					trimLine(line2, args[OPT_ARGS_TRIM].get_string());
			}
		}
		if(++cmpCnt&&(diffLoc=misMatch(line1,line2))&&++diffCnt)
		{
			flag = false;
			if (args[OPT_ARGS_DISPLAY].existed() && (args[OPT_ARGS_DISPLAY].get_string() == "normal" || args[OPT_ARGS_DISPLAY].get_string() == "detailed")) {
				cout << "第[" << line_num1 << " / " << line_num2 << "]行 - ";
				if (diffLoc > line1.length()) {
					cout << "文件2的尾部有多余字符：" << endl;
				}
				else if (diffLoc > line2.length()) {
					cout << "文件1的尾部有多余字符：" << endl;
				}
				else {
					cout << "第[" << diffLoc - 1 << "]个字符开始有差异："<<endl;
				}
				if(args[OPT_ARGS_DISPLAY].get_string() == "detailed")
					printRuler(max(line1.length(), line2.length()), 8);
				cout << "文件1 : ";
				if(!file1.eof()||!line1.empty())
					printDiffLine(line1, line2);
				cout<<(file1.eof()?"<EOF>":"<CR>")<<endl;
				cout << "文件2 : ";
				if(!file2.eof()||!line2.empty())
					printDiffLine(line2, line1);
				cout << (file2.eof() ? "<EOF>" : "<CR>") << endl;
				if (args[OPT_ARGS_DISPLAY].get_string() == "detailed") {
					cout << "文件1(HEX) : "<<endl;
					if(line1.length())
						printHexLine(line1);
					else if(!file1.eof())
						cout << "<EMPTY>" << endl;
					else
						cout << "<EOF>" << endl;
					cout << "文件2(HEX) : " << endl;
					if(line2.length())
						printHexLine(line2);
					else if(!file2.eof())
						cout << "<EMPTY>" << endl;
					else
						cout << "<EOF>" << endl;

				}
				cout<<endl;
			}

		}
		else if (file1.eof() ^ file2.eof()) {
			if (args[OPT_ARGS_IGNORE_BLANK].existed()) {
				bool leftBlank = true;
				while (!file1.eof()) {
					getline(file1, line1);
					if(args[OPT_ARGS_TRIM].existed())
						trimLine(line1, args[OPT_ARGS_TRIM].get_string());
					if (!line1.empty()) {
						leftBlank = false;
						break;
					}
				}
				if (leftBlank) {
					while (!file2.eof()) {
						getline(file2, line2);
						if(args[OPT_ARGS_TRIM].existed())
							trimLine(line2, args[OPT_ARGS_TRIM].get_string());
						if (!line2.empty()) {
							leftBlank = false;
							break;
						}
					}
				}
				if (leftBlank)
					break;
			}
			++diffCnt;
			flag = false;
			if (args[OPT_ARGS_DISPLAY].existed() && (args[OPT_ARGS_DISPLAY].get_string() == "normal" || args[OPT_ARGS_DISPLAY].get_string() == "detailed")) {
				cout << "第[" << line_num1 << " / " << line_num2 << "]行 - ";
				if (file1.eof()) {
					cout << "文件1已结束/文件2仍有内容：" << endl;
				}
				else {
					cout << "文件1仍有内容/文件2已结束：" << endl;
				}
				if(args[OPT_ARGS_DISPLAY].get_string() == "detailed")
					printRuler(max(line1.length(), line2.length()), 8);
				cout << "文件1 : ";
				if(!file1.eof()||!line1.empty())
					printDiffLine(line1, line2);
				cout << (file1.eof() ? "<EOF>" : "<CR>") << endl;
				cout << "文件2 : ";
				if(!file2.eof()||!line2.empty())
					printDiffLine(line2, line1);
				cout << (file2.eof() ? "<EOF>" : "<CR>") << endl;
				if (args[OPT_ARGS_DISPLAY].get_string() == "detailed") {
					cout << "文件1(HEX) : " << endl;
					if(line1.length())
						printHexLine(line1);
					else if(!file1.eof())
						cout << "<EMPTY>" << endl;
					else
						cout << "<EOF>" << endl;
					cout << "文件2(HEX) : " << endl;
					if(line2.length())
						printHexLine(line2);
					else if(!file2.eof())
						cout << "<EMPTY>" << endl;
					else
						cout << "<EOF>" << endl;

				}
				cout << endl;
			}
		}
	}
	if (args[OPT_ARGS_DISPLAY].existed() && (args[OPT_ARGS_DISPLAY].get_string() == "normal" || args[OPT_ARGS_DISPLAY].get_string() == "detailed")) {
		if(flag)
			cout << "在指定检查条件下完全一致." << endl;
		else{
			cout << setfill('=') << setw(width) << "" << setfill(' ') << endl;
			cout << "在指定检查条件下共" << diffCnt << "行有差异";
			if (args[OPT_ARGS_MAX_DIFFNUM].existed() && args[OPT_ARGS_MAX_DIFFNUM].get_int() == diffCnt)
				cout << "[已到设定的最大差异值]";
			cout << "."<<endl;
			cout << "阅读提示：" << endl;
			cout << "\t1、空行用<EMPTY>标出" << endl;
			cout << "\t2、文件结束用<EOF>标出" << endl;
			cout << "\t3、两行相同列位置的差异字符用亮色标出" << endl;
			cout << "\t4、每行中的CR/LF/VT/BS/BEL用X标出(方便看清隐含字符)" << endl;
			cout << "\t5、每行尾的多余的字符用亮色标出，CR/LF/VT/BS/BEL用亮色X标出(方便看清隐含字符)" << endl;
			cout << "\t6、每行最后用<CR>标出(方便看清隐含字符)" << endl;
			cout << "\t7、中文因为编码问题，差异位置可能报在后半个汉字上，但整个汉字都亮色标出" << endl;
			if (args[OPT_ARGS_DISPLAY].get_string() != "detailed")
				cout << "\t8、用--display detailed可以得到更详细的信息" << endl;
		}
		cout << setfill('=') << setw(width) << "" << setfill(' ') << endl;
	}
	else
		cout<<(flag?"文件相同.":"文件不同.")<<endl;
	return 0;
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
static void usage(const char* const argv0)
{
	char procname[256];

	/* 从"xx\\xx\\txt_compare.exe形式的argv[0]中截取txt_compare出来" */
	getProcname(procname, argv0);

	ostringstream msg;
	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;
	cout << endl;

	msg << procname << " [Ver1.0.1]" << endl;
	msg << endl;

	msg << "Usage: " << procname << " --file1 xxx --file2 xxx [ --trim none/left/rigth/all | --lineskip 0..100 | --lineoffset -100..100 | --ignore_blank | --max_diff 0..100 | --display none/normal/detailed ]" << endl;

	msg << setw(wkey) << ' ' << "必选项：" << endl;
	msg << setw(wopt) << ' ' << "--file1 name1              : 指定要比较的第1个文件名（必选）" << endl;
	msg << setw(wopt) << ' ' << "--file2 name2              : 指定要比较的第2个文件名（必选）" << endl;
	msg << endl;

	msg << setw(wkey) << ' ' << "可选项：" << endl;
	msg << setw(wopt) << ' ' << "--trim none/left/right/all     : 指定每行的空格/tab的忽略方式（无此项则默认为none）" << endl;
	msg << setw(wopt) << ' ' << "\tnone   : 每行均严格匹配" << endl;
	msg << setw(wopt) << ' ' << "\tleft   : 每行均忽略头部空格" << endl;
	msg << setw(wopt) << ' ' << "\tright  : 每行均忽略尾部空格" << endl;
	msg << setw(wopt) << ' ' << "\tall    : 每行均忽略头尾空格" << endl;
	msg << setw(wopt) << ' ' << "--lineskip s                   : 指定文件的前s行不比较（无此项则s为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[0..100]，表示同时跳过两个文件的前s行" << endl;
	msg << setw(wopt) << ' ' << "--lineoffset n                 : 指定文件错位的行数（无此项则n为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[-100..100]，负数表示忽略file1的前n行，正数表示忽略file2的前n行" << endl;
	msg << setw(wopt) << ' ' << "--ignore_blank                 : 忽略文件中--trim后的所有空行(无此项则不忽略)" << endl;
	msg << setw(wopt) << ' ' << "--CR_CRLF_not_equal            : 不忽略Windows/Linux文件的换行符差异(无此项则忽略差异)" << endl;
	msg << setw(wopt) << ' ' << "--max_diff m                   : 指定文件错位的行数（无此项则m为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[0..100]，表示满m个不同行则结束运行，不再进行后续的比较" << endl;
	msg << setw(wopt) << ' ' << "--max_line x                   : 指定文件比较的最大行数（无此项则x为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[0..10000]，表示比较x行后则结束运行，不再进行后续的比较" << endl;
	msg << setw(wopt) << ' ' << "--display none/normal/detailed : 指定显示的信息（无此项则默认为none）" << endl;
	msg << setw(wopt) << ' ' << "\tnone     : 仅一行匹配与否的提示" << endl;
	msg << setw(wopt) << ' ' << "\tnormal   : 每个差异行给出差异信息" << endl;
	msg << setw(wopt) << ' ' << "\tdetailed : 每个差异行给出更详细的差异信息" << endl;
	msg << endl;

	msg << "e.g.   " << procname << " --file1 my.txt --file2 std.txt" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim all" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，每行均去除头尾空格，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --lineoffset -2" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略my.txt的前2行(即my.txt的第3行与std.txt的第1行进行比较，每行严格匹配)，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim left --lineoffset 3" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略std.txt的前3行(即my.txt的第1行与std.txt的第4行进行比较)，每行去除头部空格，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略文件中的所有空行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --lineskip 1" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，第一行不比较" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank --trim right" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略文件中去除头尾空格后的所有空行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_diff 3" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，有3行不匹配后结束运行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_line 7" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，仅比较前7行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --CR_CRLF_not_queal" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，不忽略Windows/Linux的文件差异，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display normal" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，不匹配的行显示信息" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display detailed" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，不匹配的行显示详细信息" << endl;
	msg << endl;

	/* 一次全部显示出来 */ 
	cout << msg.str() << endl;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char** argv)
{
	/* 指定去除空格的方式 */
	const string TrimType[] = {	"none",	"left",	"right",	"all", "" };
	const string DisplayType[] = { "none",	"normal",	"detailed", "" };

	args_analyse_tools args[] = {
		args_analyse_tools("--help",			ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--debug",			ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--file1",			ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--file2",			ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--trim",			ST_EXTARGS_TYPE::str_with_set_error, 1, 0, TrimType),
		args_analyse_tools("--lineskip",		ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 100),
		args_analyse_tools("--lineoffset",		ST_EXTARGS_TYPE::int_with_default,   1, 0, -100, 100),
		args_analyse_tools("--ignore_blank",	ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--CR_CRLF_not_equal",ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--max_diff",		ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 100),
		args_analyse_tools("--max_line",		ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 10000),
		args_analyse_tools("--display",			ST_EXTARGS_TYPE::str_with_set_error, 1, 0, DisplayType),
		args_analyse_tools()  //最后一个，用于结束
	};
	int cur_argc, ret = 0;

	//最后一个参数1，表示除选项参数外，还有其它参数
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//错误信息在函数中已打印
		args_analyse_print(args);
		usage(argv[0]);
		return -1;
	}

	/* 对help做特殊处理 */
	if (args[OPT_ARGS_HELP].existed()) {
		//只要有 --help，其它参数都忽略，显示帮助即可
		args_analyse_print(args);
		usage(argv[0]);
		return -1; //执行完成直接退出
	}

	if (args[OPT_ARGS_FILE1].existed() + args[OPT_ARGS_FILE2].existed() != 2) {
		usage(argv[0]);
		cout << "必须指定参数[" << args[OPT_ARGS_FILE1].get_name() << "和"<< args[OPT_ARGS_FILE2].get_name() << "]" << endl;
		return -1;
	}

	if (args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed() && (args[OPT_ARGS_TRIM].get_string() == "right" || args[OPT_ARGS_TRIM].get_string() == "all")) {
		usage(argv[0]);
		cout << "参数[" << args[OPT_ARGS_CR_CRLF_NOT_EQUAL].get_name() << "]不能和[" << args[OPT_ARGS_TRIM].get_name() << " right/all]同时存在." << endl;
		return -1;
	}

	if (args[OPT_ARGS_DEBUG].existed())
		args_analyse_print(args);

	/* 进入实际的功能调用，完成相应的功能 */ 
	compDiff(args);

	return 0;
}

/* 
阅读提示：
1、空行用 xxx 标出
2、文件结束用 xxx 标出
3、两行相同列位置的差异字符用亮色标出
4、每行中的CR/LF/VT/BS/BEL用X标出(方便看清隐含字符)
5、每行尾的多余的字符用亮色标出，CR/LF/VT/BS/BEL用亮色X标出(方便看清隐含字符)
6、每行最后用 xxx 标出(方便看清隐含字符)"
7、中文因为编码问题，差异位置可能报在后半个汉字上，但整个汉字都亮色标出
8、用--display detailed可以得到更详细的信息
*/ 

#if 0
    //分隔行===的宽度计算 
	int width = (this->line_maxlen / 10 + 1) * 10 + 8 + 2; //比---的标尺多2个：line_maxlen向上取10倍整数，8是"文件1 : "

	/* 如果加了hex输出，则最小宽度为80 */
	if (this->display_detailed && width < 80)
		width = 80;
#endif

/* 测试文件准备
	1.txt ：正常文件
	2.txt ：在1.txt的基础上，某些行前面无空格
	3.txt ：在1.txt的基础上，某些行后面无空格
	4.txt ：在1.txt的基础上，最后一行没有回车
	5.txt ：在1.txt的基础上，多一些空行
	6.txt ：在1.txt的基础上，多一些空行，其中某些行有空格和tab
	7.txt ：和1.txt完全相同
	8.txt ：是1.txt的Linux格式版

txt_compare --file1 1.txt --file2 2.txt --trim left
txt_compare --file1 1.txt --file2 2.txt --trim all
txt_compare --file1 2.txt --file2 3.txt --trim all
txt_compare --file1 1.txt --file2 4.txt --trim right --ignore_blank
txt_compare --file1 1.txt --file2 4.txt --trim all --ignore_blank
txt_compare --file1 2.txt --file2 4.txt --trim all --ignore_blank
txt_compare --file1 3.txt --file2 4.txt --trim right --ignore_blank
txt_compare --file1 3.txt --file2 4.txt --trim all --ignore_blank
txt_compare --file1 1.txt --file2 5.txt --trim right --ignore_blank
txt_compare --file1 1.txt --file2 6.txt --ignore_blank --trim right
txt_compare --file1 5.txt --file2 6.txt --ignore_blank --trim all
txt_compare --file1 1.txt --file2 7.txt
txt_compare --file1 1.txt --file2 8.txt

rem 不相同的例子
txt_compare --file1 1.txt --file2 2.txt
txt_compare --file1 1.txt --file2 2.txt --trim right
txt_compare --file1 1.txt --file2 5.txt
txt_compare --file1 1.txt --file2 5.txt --trim right
txt_compare --file1 1.txt --file2 6.txt
txt_compare --file1 1.txt --file2 6.txt --ignore_blank
txt_compare --file1 1.txt --file2 8.txt --CR_CRLF_not_equal

*/
