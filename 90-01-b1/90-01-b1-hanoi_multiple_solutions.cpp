/* �ƿ� 2253731 ����� */
#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "90-01-b1-hanoi.h"
using namespace std;

static int lapse = -1, top_of_A = 0, top_of_B = 0, top_of_C = 0, i = 0;
static int con_of_A[MAX] = { 0 }, con_of_B[MAX] = { 0 }, con_of_C[MAX] = { 0 };


/* ----------------------------------------------------------------------------------

	 ���ļ����ܣ�
	1����ű� hanoi_main.cpp �и��ݲ˵�����ֵ���õĸ��˵����Ӧ��ִ�к���

	 ���ļ�Ҫ��
	1�����������ⲿȫ�ֱ�����const��#define�������Ʒ�Χ�ڣ�
	2�������徲̬ȫ�ֱ�����������Ҫ��������Ҫ�����ĵ���ʾ��ȫ�ֱ�����ʹ��׼���ǣ����á����á��ܲ��þ������ã�
	3����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	4���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------------------------------------------------- */


   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ�
	 ���������
	 �� �� ֵ��
	 ˵    ����
   ***************************************************************************/
void do_lapse()
{
	if (lapse != -1)
		if (lapse)
			Sleep(lapse);
		else
			while (_getch() != '\r')
				;
}

void print_array()
{
	cout << "  A:";
	int j;
	for (j = 0; j < top_of_A; j++)
		cout << setw(2) << con_of_A[j];
	cout << setw(21 - 2 * top_of_A) << ' ';
	cout << "B:";
	for (j = 0; j < top_of_B; j++)
		cout << setw(2) << con_of_B[j];
	cout << setw(21 - 2 * top_of_B) << ' ';
	cout << "C:";
	for (j = 0; j < top_of_C; j++)
		cout << setw(2) << con_of_C[j];
	cout << setw(21 - 2 * top_of_C) << ' ';
	do_lapse();
}

void in_move_disk(char src, char tmp, char dst)
{
	switch (src) {
		case 'A':
			switch (dst) {
				case 'B':
					con_of_B[top_of_B++] = con_of_A[--top_of_A];
					break;
				case 'C':
					con_of_C[top_of_C++] = con_of_A[--top_of_A];
			}
			break;
		case 'B':
			switch (dst) {
				case 'A':
					con_of_A[top_of_A++] = con_of_B[--top_of_B];
					break;
				case 'C':
					con_of_C[top_of_C++] = con_of_B[--top_of_B];
			}
			break;
		case 'C':
			switch (dst) {
				case 'A':
					con_of_A[top_of_A++] = con_of_C[--top_of_C];
					break;
				case 'B':
					con_of_B[top_of_B++] = con_of_C[--top_of_C];
			}
	}
}

void out_move_disk1(char src, char dst)
{
	switch (src) {
		case 'A':
			cct_gotoxy(11, 29 - top_of_A);
			cout << setw(2) << ' ';
			switch (dst) {
				case 'B':
					cct_gotoxy(21, 30 - top_of_B);
					cout << setw(2) << con_of_B[top_of_B - 1];
					break;
				case 'C':
					cct_gotoxy(31, 30 - top_of_C);
					cout << setw(2) << con_of_C[top_of_C - 1];
			}
			break;
		case 'B':
			cct_gotoxy(21, 29 - top_of_B);
			cout << setw(2) << ' ';
			switch (dst) {
				case 'A':
					cct_gotoxy(11, 30 - top_of_A);
					cout << setw(2) << con_of_A[top_of_A - 1];
					break;
				case 'C':
					cct_gotoxy(31, 30 - top_of_C);
					cout << setw(2) << con_of_C[top_of_C - 1];
			}
			break;
		case 'C':
			cct_gotoxy(31, 29 - top_of_C);
			cout << setw(2) << ' ';
			switch (dst) {
				case 'A':
					cct_gotoxy(11, 30 - top_of_A);
					cout << setw(2) << con_of_A[top_of_A - 1];
					break;
				case 'B':
					cct_gotoxy(21, 30 - top_of_B);
					cout << setw(2) << con_of_B[top_of_B - 1];
			}
	}
	do_lapse();
}

void move_a_disk(int x_spt, int y_spt, int x_dst, int y_dst, int width, int color)
{
	int top = 1, x = x_spt, y, direction = (x_dst < x_spt ? -1 : 1);
	for (y = y_spt; y > top; y--) {
		cct_showstr(x - width, y - 1, "", color, 14 - color, 1, 2 * width + 1);
		cct_showstr(x - width, y, "", 0, 7, 1, 2 * width + 1);
		if (y >= 3)
			cct_showch(x, y, ' ', COLOR_HYELLOW, COLOR_BLUE, 1);
		Sleep(10);
	}
	for (x = x_spt; x != x_dst; x += direction) {
		cct_showch(x - direction * width, y, ' ', 0, 7, 1);
		cct_showch(x + direction * (width + 1), y, ' ', color, 14 - color, 1);
		Sleep(10);
	}
	for (; y < y_dst; y++) {
		cct_showstr(x - width, y, "", 0, 7, 1, 2 * width + 1);
		cct_showstr(x - width, y + 1, "", color, 14 - color, 1, 2 * width + 1);
		if (y >= 3)
			cct_showch(x, y, ' ', COLOR_HYELLOW, COLOR_BLUE, 1);
		Sleep(10);
	}
	cct_setcolor();
}

void out_move_disk2(char src, char dst, int n, int n_max)
{
	int width = n, x1 = 32 * (src - 'A') + 12, x2 = 32 * (dst - 'A') + 12, color = (n_max - n + 1 >= 7 ? n_max - n + 2 : n_max - n + 1);
	switch (src) {
		case 'A':
			switch (dst) {
				case 'B':
					move_a_disk(x1, 14 - top_of_A, x2, 15 - top_of_B, width, color);
					break;
				case 'C':
					move_a_disk(x1, 14 - top_of_A, x2, 15 - top_of_C, width, color);
			}
			break;
		case 'B':
			switch (dst) {
				case 'A':
					move_a_disk(x1, 14 - top_of_B, x2, 15 - top_of_A, width, color);
					break;
				case 'C':
					move_a_disk(x1, 14 - top_of_B, x2, 15 - top_of_C, width, color);
			}
			break;
		case 'C':
			switch (dst) {
				case 'B':
					move_a_disk(x1, 14 - top_of_C, x2, 15 - top_of_B, width, color);
					break;
				case 'A':
					move_a_disk(x1, 14 - top_of_C, x2, 15 - top_of_A, width, color);
			}
	}
}

void input_parameters(char* src, char* dst, char* tmp, int* n, const int option)
{
	while (1) {
		cout << "�����뺺ŵ���Ĳ���(1-10)" << endl;
		cin >> *n;
		cin.clear();
		cin.ignore(23333, '\n');
		if (*n > 0 && *n < MAX + 1)
			break;
	}
	while (1) {
		cout << "��������ʼ��(A-C)" << endl;
		cin >> *src;
		*src = (*src > 67 ? *src - 32 : *src);
		cin.clear();
		cin.ignore(23333, '\n');
		if (*src > 64 && *src < 68)
			break;
	}
	while (1) {
		cout << "������Ŀ����(A-C)" << endl;
		cin >> *dst;
		*dst = (*dst > 67 ? *dst - 32 : *dst);
		cin.clear();
		cin.ignore(23333, '\n');
		if (*dst > 64 && *dst < 68 && *dst != *src)
			break;
		if (*dst == *src)
			cout << "Ŀ����(" << *dst << ")��������ʼ��(" << *src << ")��ͬ" << endl;
	}
	*tmp = 'A' + 'B' + 'C' - *src - *dst;
	if (option == 8 || option == 4) {
		while (1) {
			cout << "�������ƶ��ٶ�(0-5: 0-���س�������ʾ 1-��ʱ� 5-��ʱ���)" << endl;
			cin >> lapse;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(2253731, '\n');
				continue;
			}
			cin.ignore(2253731, '\n');
			if (lapse >= 0 && lapse < 6)
				break;
		}
		lapse = (lapse ? 300 - 50 * lapse : 0);
	}
}

void print_tower(int sc, int n)
{
	int h;
	cct_gotoxy(0, 34);
	cout << "��ʼ:                ";
	print_array();
	cct_gotoxy(10, 30);
	cout << setfill('=') << setw(25) << '=' << setfill(' ');
	cct_gotoxy(12, 31);
	putchar('A');
	cct_gotoxy(22, 31);
	putchar('B');
	cct_gotoxy(32, 31);
	putchar('C');
	for (h = 1; h <= n; h++) {
		cct_gotoxy(sc, 30 - h);
		cout << setw(2) << n + 1 - h;
	}
	do_lapse();
}

void print_elementary_solution(char src, char dst, int i, int n, int option)
{
	if (option == 4 || option == 8)
		cct_gotoxy(0, 34);
	if (option == 2 || option == 3 || option == 4 || option == 8)
		cout << "��" << setw(4) << i << " ��(";
	cout << setw(2) << n << "# " << src << "-->" << dst;
	if (option == 2 || option == 3 || option == 4 || option == 8)
		cout << ')';
	if (option == 3 || option == 4 || option == 8) {
		print_array();
	}
	cout << endl;
}

void move_hanoi(char src, char tmp, char dst, int n, int& option, int n_max)
{
	if(option==-1)
		return;
	in_move_disk(src, tmp, dst);
	switch (option) {
		case 7:
		case 8:
			out_move_disk2(src, dst, n, n_max);
			if (option == 7){
				option = -1;
				return;
			}
		default:
			print_elementary_solution(src, dst, ++i, n, option);
			if (option != 8 && option != 4)
				break;
			out_move_disk1(src, dst);
	}
}

void hanoi_solution(int n, char src, char tmp, char dst, int& option, int n_max)//�ݹ麯��
{
	if (n == 1) {
		move_hanoi(src, tmp, dst, n, option, n_max);
	}
	else {
		hanoi_solution(n - 1, src, dst, tmp, option, n_max);
		move_hanoi(src, tmp, dst, n, option, n_max);
		hanoi_solution(n - 1, tmp, src, dst, option, n_max);
	}
}

int set_initial_state(char src, int n)
{
	int c, sc = 0;
	switch (src) {
		case 'A':
			sc = 11;
			top_of_A = n;
			for (c = 0; c < n; c++)
				con_of_A[n - 1 - c] = c + 1;
			break;
		case 'B':
			sc = 21;
			top_of_B = n;
			for (c = 0; c < n; c++)
				con_of_B[n - 1 - c] = c + 1;
			break;
		case 'C':
			sc = 31;
			top_of_C = n;
			for (c = 0; c < n; c++)
				con_of_C[n - 1 - c] = c + 1;
			break;
	}
	return sc;
}

void print_column(int option)//������������Ϊ (32k+12,15)
{
	int i = 0, j = 1, h;
	for (i = 0; i < 3; i++) {
		cct_showstr(32 * i + 1, 15, "", COLOR_HYELLOW, COLOR_BLUE, 1, 23);
		for (h = 3; h < 15; h++) {
			cct_showch(32 * i + 12, h, ' ', COLOR_HYELLOW, COLOR_BLUE);
			if (option == 5)
				Sleep(100);
		}
	}
	cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}

void print_initial_disks(char src, int n)
{
	int x = 32 * (src - 'A') + 12, y = 14 - n, color;
	for (color = 1; n >= 1; n--, color++) {
		if (color == 7)
			color++;
		cct_showstr(x - n, y + n, "", color, 14 - color, 1, 2 * n + 1);
		Sleep(100);
	}
	cct_setcolor();
}

void input_player(char input[])
{
	while (1) {
		cct_showstr(60, 36, "", 0, 7, 1, 30);
		cct_showstr(0, 37, "", 0, 7, 1, 30);
		cct_gotoxy(60, 36);
		int count = 0, reset_flag = 0;
		for (; (input[count] = _getch()) || input[count] == 0; count++) {
			if (input[count] >= 33 && input[count] < 127)
				cout << input[count];
			else if (input[count] > 0 && input[count] != '\r') {
				count--;
				continue;
			}
			else if (input[count] <= 0) {
				input[count--] = _getch();
				continue;
			}
			if (count == 19) {
				reset_flag = 1;
				break;
			}
			if (input[count] == '\r')
				break;
			if (input[count] >= 'a' && input[count] <= 'c')
				input[count] -= 32;
		}
		if (count == 1 && (input[0] == 'Q' || input[0] == 'q')) {
			input[0] = -1;
			return;
		}
		if (reset_flag || count != 2 || input[0] < 'A' || input[0]>'C' || input[1] < 'A' || input[1]>'C' || input[0] == input[1])
			continue;
		switch (input[0]) {
			case 'A':
				if (top_of_A == 0)
					reset_flag = 2;
				else if (input[1] == 'B') {
					if (top_of_B != 0 && con_of_A[top_of_A - 1] > con_of_B[top_of_B - 1])
						reset_flag = 1;
				}
				else if (top_of_C != 0 && con_of_A[top_of_A - 1] > con_of_C[top_of_C - 1])
					reset_flag = 1;
				break;
			case 'B':
				if (top_of_B == 0)
					reset_flag = 2;
				else if (input[1] == 'A') {
					if (top_of_A != 0 && con_of_B[top_of_B - 1] > con_of_A[top_of_A - 1])
						reset_flag = 1;
				}
				else if (top_of_C != 0 && con_of_B[top_of_B - 1] > con_of_C[top_of_C - 1])
					reset_flag = 1;
				break;
			case 'C':
				if (top_of_C == 0)
					reset_flag = 2;
				else if (input[1] == 'B') {
					if (top_of_B != 0 && con_of_C[top_of_C - 1] > con_of_B[top_of_B - 1])
						reset_flag = 1;
				}
				else if (top_of_A != 0 && con_of_C[top_of_C - 1] > con_of_A[top_of_A - 1])
					reset_flag = 1;
		}
		if (reset_flag)
			cout << endl << (reset_flag == 1 ? "����ѹС�̣��Ƿ��ƶ���" : "Դ��Ϊ�գ�");
		else
			break;
		Sleep(500);
	}
}

void hanoi_player(const char src, const char dst, const int n)
{
	cct_gotoxy(0, 36);
	cout << "�������ƶ�������(������ʽ��AC=A���˵������ƶ���C��Q=�˳�) ��";
	while (1) {
		char input[20] = { 0 };
		input_player(input);
		switch (input[0]) {
			case -1:
				cout << endl << "��Ϸ��ֹ����������";
				return;
			case 'A':
				if (input[1] == 'B') {
					in_move_disk('A', 'C', 'B');
					cct_gotoxy(0, 34);
					print_array();
					out_move_disk1('A', 'B');
					out_move_disk2('A', 'B', con_of_A[top_of_A], n);
				}
				else {
					in_move_disk('A', 'B', 'C');
					cct_gotoxy(0, 34);
					print_array();
					out_move_disk1('A', 'C');
					out_move_disk2('A', 'C', con_of_A[top_of_A], n);
				}
				break;
			case 'B':
				if (input[1] == 'A') {
					in_move_disk('B', 'C', 'A');
					cct_gotoxy(0, 34);
					print_array();
					out_move_disk1('B', 'A');
					out_move_disk2('B', 'A', con_of_B[top_of_B], n);
				}
				else {
					in_move_disk('B', 'A', 'C');
					cct_gotoxy(0, 34);
					print_array();
					out_move_disk1('B', 'C');
					out_move_disk2('B', 'C', con_of_B[top_of_B], n);
				}
				break;
			case 'C':
				if (input[1] == 'B') {
					in_move_disk('C', 'A', 'B');
					cct_gotoxy(0, 34);
					print_array();
					out_move_disk1('C', 'B');
					out_move_disk2('C', 'B', con_of_C[top_of_C], n);
				}
				else {
					in_move_disk('C', 'B', 'A');
					cct_gotoxy(0, 34);
					print_array();
					out_move_disk1('C', 'A');
					out_move_disk2('C', 'A', con_of_C[top_of_C], n);
				}
		}
		switch (dst) {
			case 'A':
				if (top_of_A == n) {
					cct_gotoxy(0, 37);
					cout << "��Ϸ������";
					return;
				}
				break;
			case 'B':
				if (top_of_B == n) {
					cct_gotoxy(0, 37);
					cout << "��Ϸ������";
					return;
				}
				break;
			case 'C':
				if (top_of_C == n) {
					cct_gotoxy(0, 37);
					cout << "��Ϸ������";
					return;
				}
		}
	}
}

void hanoi(int option)
{
	top_of_A = 0;
	top_of_C = 0;
	top_of_B = 0;
	i = 0;
	lapse = -1;
	int c = 0;
	for (c = 0; c < MAX; c++)
		con_of_A[c] = 0;
	for (c = 0; c < MAX; c++)
		con_of_B[c] = 0;
	for (c = 0; c < MAX; c++)
		con_of_C[c] = 0;
	cct_gotoxy(0, 15);
	int n;
	char src, dst, tmp;
	input_parameters(&src, &dst, &tmp, &n, option);
	int sc = set_initial_state(src, n);
	if (option >= 4)
		cct_cls();
	if (option == 4 || option == 8 || option == 9) {
		cout << "�� " << src << " �ƶ��� " << dst << "���� " << n << " ��";
		if (option != 9)
			cout << " ����ʱ����Ϊ " << (lapse ? 6 - lapse / 50 : 0) << endl;
		print_tower(sc, n);
	}
	if (option >= 6) {
		print_column(option);
		print_initial_disks(src, n);
	}
	do_lapse();
	if (option != 6 && option != 9)
		hanoi_solution(n, src, tmp, dst, option, n);
	if (option == 9)
		hanoi_player(src, dst, n);
	return;
}