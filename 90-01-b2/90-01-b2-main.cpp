/* 2253731 �ƿ� ����� */
#include "90-01-b2.h"
#include "../include/menu.h"
using namespace std;
const char ops[7][100] = { "�������ҳ����������ʶ",
"���������һ���������ֲ�����ʾ��",
"���������һ�أ��ֲ�����ʾ��",
"αͼ�ν����������ѡ��һ��ɫ�飨�޷ָ��ߣ�",
"αͼ�ν����������ѡ��һ��ɫ�飨�зָ��ߣ�",
"αͼ�ν������һ���������ֲ��裩",
"αͼ�ν���������" };

int main()
{
	while(1){
		char choice = menu("char",ops,7);
		if (choice == 'Q')
			return 0;
		int row, column, stars[N + 2][N + 2] = { 0 },ret=0;
		char rc[3] = { 0 };
		cct_cls();
		set_size(row, column);
		do{
			set_initial_state(row, column, stars);
			switch (choice) {
				case 'A':
				case 'B':
				case 'C':
					ret = 0;
					inside(row, column, stars, rc, choice);
					break;
				case 'D':
				case 'E':
				case 'F':
				case 'G':
					ret=popstar_interface(row, column, stars, choice);
					break;
			}
		} while (ret);
	}
}