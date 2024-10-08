/* 2253731 �ƿ� ����� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//���������Ҫ��ͷ�ļ� 
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





//��1��ʼ����
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
	//�ָ���===�Ŀ�ȼ��� 
	int width = (maxLen / 10 + 1) * 10 + 8 + 2; //��---�ı�߶�2����line_maxlen����ȡ10��������8��"�ļ�1 : "

	/* �������hex���������С���Ϊ80 */
	if (args[OPT_ARGS_DISPLAY].existed() && args[OPT_ARGS_DISPLAY].get_string()=="detailed" && width < 80)
		width = 80;
	if (args[OPT_ARGS_DISPLAY].existed() && (args[OPT_ARGS_DISPLAY].get_string() == "normal" || args[OPT_ARGS_DISPLAY].get_string() == "detailed")){
		cout << "�ȽϽ�������" << endl;
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
				cout << "��[" << line_num1 << " / " << line_num2 << "]�� - ";
				if (diffLoc > line1.length()) {
					cout << "�ļ�2��β���ж����ַ���" << endl;
				}
				else if (diffLoc > line2.length()) {
					cout << "�ļ�1��β���ж����ַ���" << endl;
				}
				else {
					cout << "��[" << diffLoc - 1 << "]���ַ���ʼ�в��죺"<<endl;
				}
				if(args[OPT_ARGS_DISPLAY].get_string() == "detailed")
					printRuler(max(line1.length(), line2.length()), 8);
				cout << "�ļ�1 : ";
				if(!file1.eof()||!line1.empty())
					printDiffLine(line1, line2);
				cout<<(file1.eof()?"<EOF>":"<CR>")<<endl;
				cout << "�ļ�2 : ";
				if(!file2.eof()||!line2.empty())
					printDiffLine(line2, line1);
				cout << (file2.eof() ? "<EOF>" : "<CR>") << endl;
				if (args[OPT_ARGS_DISPLAY].get_string() == "detailed") {
					cout << "�ļ�1(HEX) : "<<endl;
					if(line1.length())
						printHexLine(line1);
					else if(!file1.eof())
						cout << "<EMPTY>" << endl;
					else
						cout << "<EOF>" << endl;
					cout << "�ļ�2(HEX) : " << endl;
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
				cout << "��[" << line_num1 << " / " << line_num2 << "]�� - ";
				if (file1.eof()) {
					cout << "�ļ�1�ѽ���/�ļ�2�������ݣ�" << endl;
				}
				else {
					cout << "�ļ�1��������/�ļ�2�ѽ�����" << endl;
				}
				if(args[OPT_ARGS_DISPLAY].get_string() == "detailed")
					printRuler(max(line1.length(), line2.length()), 8);
				cout << "�ļ�1 : ";
				if(!file1.eof()||!line1.empty())
					printDiffLine(line1, line2);
				cout << (file1.eof() ? "<EOF>" : "<CR>") << endl;
				cout << "�ļ�2 : ";
				if(!file2.eof()||!line2.empty())
					printDiffLine(line2, line1);
				cout << (file2.eof() ? "<EOF>" : "<CR>") << endl;
				if (args[OPT_ARGS_DISPLAY].get_string() == "detailed") {
					cout << "�ļ�1(HEX) : " << endl;
					if(line1.length())
						printHexLine(line1);
					else if(!file1.eof())
						cout << "<EMPTY>" << endl;
					else
						cout << "<EOF>" << endl;
					cout << "�ļ�2(HEX) : " << endl;
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
			cout << "��ָ�������������ȫһ��." << endl;
		else{
			cout << setfill('=') << setw(width) << "" << setfill(' ') << endl;
			cout << "��ָ����������¹�" << diffCnt << "���в���";
			if (args[OPT_ARGS_MAX_DIFFNUM].existed() && args[OPT_ARGS_MAX_DIFFNUM].get_int() == diffCnt)
				cout << "[�ѵ��趨��������ֵ]";
			cout << "."<<endl;
			cout << "�Ķ���ʾ��" << endl;
			cout << "\t1��������<EMPTY>���" << endl;
			cout << "\t2���ļ�������<EOF>���" << endl;
			cout << "\t3��������ͬ��λ�õĲ����ַ�����ɫ���" << endl;
			cout << "\t4��ÿ���е�CR/LF/VT/BS/BEL��X���(���㿴�������ַ�)" << endl;
			cout << "\t5��ÿ��β�Ķ�����ַ�����ɫ�����CR/LF/VT/BS/BEL����ɫX���(���㿴�������ַ�)" << endl;
			cout << "\t6��ÿ�������<CR>���(���㿴�������ַ�)" << endl;
			cout << "\t7��������Ϊ�������⣬����λ�ÿ��ܱ��ں��������ϣ����������ֶ���ɫ���" << endl;
			if (args[OPT_ARGS_DISPLAY].get_string() != "detailed")
				cout << "\t8����--display detailed���Եõ�����ϸ����Ϣ" << endl;
		}
		cout << setfill('=') << setw(width) << "" << setfill(' ') << endl;
	}
	else
		cout<<(flag?"�ļ���ͬ.":"�ļ���ͬ.")<<endl;
	return 0;
}
/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
static void usage(const char* const argv0)
{
	char procname[256];

	/* ��"xx\\xx\\txt_compare.exe��ʽ��argv[0]�н�ȡtxt_compare����" */
	getProcname(procname, argv0);

	ostringstream msg;
	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;
	cout << endl;

	msg << procname << " [Ver1.0.1]" << endl;
	msg << endl;

	msg << "Usage: " << procname << " --file1 xxx --file2 xxx [ --trim none/left/rigth/all | --lineskip 0..100 | --lineoffset -100..100 | --ignore_blank | --max_diff 0..100 | --display none/normal/detailed ]" << endl;

	msg << setw(wkey) << ' ' << "��ѡ�" << endl;
	msg << setw(wopt) << ' ' << "--file1 name1              : ָ��Ҫ�Ƚϵĵ�1���ļ�������ѡ��" << endl;
	msg << setw(wopt) << ' ' << "--file2 name2              : ָ��Ҫ�Ƚϵĵ�2���ļ�������ѡ��" << endl;
	msg << endl;

	msg << setw(wkey) << ' ' << "��ѡ�" << endl;
	msg << setw(wopt) << ' ' << "--trim none/left/right/all     : ָ��ÿ�еĿո�/tab�ĺ��Է�ʽ���޴�����Ĭ��Ϊnone��" << endl;
	msg << setw(wopt) << ' ' << "\tnone   : ÿ�о��ϸ�ƥ��" << endl;
	msg << setw(wopt) << ' ' << "\tleft   : ÿ�о�����ͷ���ո�" << endl;
	msg << setw(wopt) << ' ' << "\tright  : ÿ�о�����β���ո�" << endl;
	msg << setw(wopt) << ' ' << "\tall    : ÿ�о�����ͷβ�ո�" << endl;
	msg << setw(wopt) << ' ' << "--lineskip s                   : ָ���ļ���ǰs�в��Ƚϣ��޴�����sΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[0..100]����ʾͬʱ���������ļ���ǰs��" << endl;
	msg << setw(wopt) << ' ' << "--lineoffset n                 : ָ���ļ���λ���������޴�����nΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[-100..100]��������ʾ����file1��ǰn�У�������ʾ����file2��ǰn��" << endl;
	msg << setw(wopt) << ' ' << "--ignore_blank                 : �����ļ���--trim������п���(�޴����򲻺���)" << endl;
	msg << setw(wopt) << ' ' << "--CR_CRLF_not_equal            : ������Windows/Linux�ļ��Ļ��з�����(�޴�������Բ���)" << endl;
	msg << setw(wopt) << ' ' << "--max_diff m                   : ָ���ļ���λ���������޴�����mΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[0..100]����ʾ��m����ͬ����������У����ٽ��к����ıȽ�" << endl;
	msg << setw(wopt) << ' ' << "--max_line x                   : ָ���ļ��Ƚϵ�����������޴�����xΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[0..10000]����ʾ�Ƚ�x�к���������У����ٽ��к����ıȽ�" << endl;
	msg << setw(wopt) << ' ' << "--display none/normal/detailed : ָ����ʾ����Ϣ���޴�����Ĭ��Ϊnone��" << endl;
	msg << setw(wopt) << ' ' << "\tnone     : ��һ��ƥ��������ʾ" << endl;
	msg << setw(wopt) << ' ' << "\tnormal   : ÿ�������и���������Ϣ" << endl;
	msg << setw(wopt) << ' ' << "\tdetailed : ÿ�������и�������ϸ�Ĳ�����Ϣ" << endl;
	msg << endl;

	msg << "e.g.   " << procname << " --file1 my.txt --file2 std.txt" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim all" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ÿ�о�ȥ��ͷβ�ո񣬽�һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --lineoffset -2" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt������my.txt��ǰ2��(��my.txt�ĵ�3����std.txt�ĵ�1�н��бȽϣ�ÿ���ϸ�ƥ��)����һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim left --lineoffset 3" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt������std.txt��ǰ3��(��my.txt�ĵ�1����std.txt�ĵ�4�н��бȽ�)��ÿ��ȥ��ͷ���ո񣬽�һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt�������ļ��е����п��У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --lineskip 1" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt����һ�в��Ƚ�" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank --trim right" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt�������ļ���ȥ��ͷβ�ո������п��У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_diff 3" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��3�в�ƥ���������У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_line 7" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬���Ƚ�ǰ7�У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --CR_CRLF_not_queal" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬������Windows/Linux���ļ����죬��һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display normal" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��ƥ�������ʾ��Ϣ" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display detailed" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��ƥ�������ʾ��ϸ��Ϣ" << endl;
	msg << endl;

	/* һ��ȫ����ʾ���� */ 
	cout << msg.str() << endl;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main(int argc, char** argv)
{
	/* ָ��ȥ���ո�ķ�ʽ */
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
		args_analyse_tools()  //���һ�������ڽ���
	};
	int cur_argc, ret = 0;

	//���һ������1����ʾ��ѡ������⣬������������
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//������Ϣ�ں������Ѵ�ӡ
		args_analyse_print(args);
		usage(argv[0]);
		return -1;
	}

	/* ��help�����⴦�� */
	if (args[OPT_ARGS_HELP].existed()) {
		//ֻҪ�� --help���������������ԣ���ʾ��������
		args_analyse_print(args);
		usage(argv[0]);
		return -1; //ִ�����ֱ���˳�
	}

	if (args[OPT_ARGS_FILE1].existed() + args[OPT_ARGS_FILE2].existed() != 2) {
		usage(argv[0]);
		cout << "����ָ������[" << args[OPT_ARGS_FILE1].get_name() << "��"<< args[OPT_ARGS_FILE2].get_name() << "]" << endl;
		return -1;
	}

	if (args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed() && (args[OPT_ARGS_TRIM].get_string() == "right" || args[OPT_ARGS_TRIM].get_string() == "all")) {
		usage(argv[0]);
		cout << "����[" << args[OPT_ARGS_CR_CRLF_NOT_EQUAL].get_name() << "]���ܺ�[" << args[OPT_ARGS_TRIM].get_name() << " right/all]ͬʱ����." << endl;
		return -1;
	}

	if (args[OPT_ARGS_DEBUG].existed())
		args_analyse_print(args);

	/* ����ʵ�ʵĹ��ܵ��ã������Ӧ�Ĺ��� */ 
	compDiff(args);

	return 0;
}

/* 
�Ķ���ʾ��
1�������� xxx ���
2���ļ������� xxx ���
3��������ͬ��λ�õĲ����ַ�����ɫ���
4��ÿ���е�CR/LF/VT/BS/BEL��X���(���㿴�������ַ�)
5��ÿ��β�Ķ�����ַ�����ɫ�����CR/LF/VT/BS/BEL����ɫX���(���㿴�������ַ�)
6��ÿ������� xxx ���(���㿴�������ַ�)"
7��������Ϊ�������⣬����λ�ÿ��ܱ��ں��������ϣ����������ֶ���ɫ���
8����--display detailed���Եõ�����ϸ����Ϣ
*/ 

#if 0
    //�ָ���===�Ŀ�ȼ��� 
	int width = (this->line_maxlen / 10 + 1) * 10 + 8 + 2; //��---�ı�߶�2����line_maxlen����ȡ10��������8��"�ļ�1 : "

	/* �������hex���������С���Ϊ80 */
	if (this->display_detailed && width < 80)
		width = 80;
#endif

/* �����ļ�׼��
	1.txt �������ļ�
	2.txt ����1.txt�Ļ����ϣ�ĳЩ��ǰ���޿ո�
	3.txt ����1.txt�Ļ����ϣ�ĳЩ�к����޿ո�
	4.txt ����1.txt�Ļ����ϣ����һ��û�лس�
	5.txt ����1.txt�Ļ����ϣ���һЩ����
	6.txt ����1.txt�Ļ����ϣ���һЩ���У�����ĳЩ���пո��tab
	7.txt ����1.txt��ȫ��ͬ
	8.txt ����1.txt��Linux��ʽ��

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

rem ����ͬ������
txt_compare --file1 1.txt --file2 2.txt
txt_compare --file1 1.txt --file2 2.txt --trim right
txt_compare --file1 1.txt --file2 5.txt
txt_compare --file1 1.txt --file2 5.txt --trim right
txt_compare --file1 1.txt --file2 6.txt
txt_compare --file1 1.txt --file2 6.txt --ignore_blank
txt_compare --file1 1.txt --file2 8.txt --CR_CRLF_not_equal

*/
