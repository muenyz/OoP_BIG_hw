/* 2253731 �ƿ� ����� */
#include <iostream>
#include "90-01-b2.h"

using namespace std;


void print_reminder(const char a[])
{
	if (!strcmp(a, "getrc"))
		cout << "������ĸ+������ʽ[����c2]����������꣺";
	else if (!strcmp(a, "no_match"))
		cout << "����ľ�������λ�ô���������ֵͬ������������" << endl;
	else if (!strcmp(a, "wrong"))
		cout << "�����������������." << endl;
	else if (!strcmp(a, "end"))
		cout << "��С�������������End����...";
	else if (!strcmp(a, "enter"))
		cout << "���س������������������..." << endl;
	else if (!strcmp(a, "illegal"))
		cout << "����ľ�������λ��Ϊ0���Ƿ�λ�ã�������������" << endl;
	else if (!strcmp(a, "once_end"))
		cout << "�����������������س���������һ�ε�����..." << endl;
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