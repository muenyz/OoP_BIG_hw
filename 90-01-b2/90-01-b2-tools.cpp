/* 2253731 计科 张皓钦 */
#include <iostream>
#include "90-01-b2.h"

using namespace std;


void print_reminder(const char a[])
{
	if (!strcmp(a, "getrc"))
		cout << "请以字母+数字形式[例：c2]输入矩阵坐标：";
	else if (!strcmp(a, "no_match"))
		cout << "输入的矩阵坐标位置处无连续相同值，请重新输入" << endl;
	else if (!strcmp(a, "wrong"))
		cout << "输入错误，请重新输入." << endl;
	else if (!strcmp(a, "end"))
		cout << "本小题结束，请输入End继续...";
	else if (!strcmp(a, "enter"))
		cout << "按回车键进行数组下落操作..." << endl;
	else if (!strcmp(a, "illegal"))
		cout << "输入的矩阵坐标位置为0（非法位置），请重新输入" << endl;
	else if (!strcmp(a, "once_end"))
		cout << "本次消除结束，按回车键继续新一次的消除..." << endl;
}

void wait_for_enter()
{
	while (_getch() != '\r')
		;
}

char* tj_strlwr(char* str)
{
	if (str == NULL)
		return NULL;
	char* i = str;
	for (; i - str < (int)strlen(str); i++)
		if (*i >= 'A' && *i <= 'Z')
			*i += 32;

	return str;
}

void getend()
{
	char end[4] = { 0 };
	print_reminder("end");
	int x, y;
	cct_getxy(x, y);
	cin.ignore(32768, '\n');
	while (1) {
		cin.get(end, 4, '\n');
		cin.clear();
		cin.ignore(25135, '\n');
		cin.clear();
		tj_strlwr(end);
		if (!strcmp(end, "end"))
			return;
		cct_showstr(x, y, "    ");
		cout << endl;
		print_reminder("wrong");
		cct_gotoxy(x, y);
	}
}

void getkbmz(char kb, char mz)
{
	while (1) {
		int MX = -1, MY = -1, MACTION = 0, key_code1 = 0, key_code2 = 0, flag = 0;
		switch (cct_read_keyboard_and_mouse(MX, MY, MACTION, key_code1, key_code2)) {
			case CCT_KEYBOARD_EVENT:
				if (key_code1 >= 'a' && key_code1 <= 'z')
					key_code1 -= 32;
				if (key_code1 == kb)
					flag = 1;
				break;
			case CCT_MOUSE_EVENT:
				if (MACTION == mz)
					flag = 1;
				break;
		}
		if (flag)
			break;
	}
}