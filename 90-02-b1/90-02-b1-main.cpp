/* 2253731 �ƿ� ����� */
#include "90-02-b1.h"
#include "../include/menu.h"

const char ops[][100] = {
	"������Ϸ����",
	"����һ������",
	"��һ��������תһ��",
	"���������������ײ�",
	"�����������������",
	"���ְ����˹���飨�����棩"
};

int main()
{
	int col, line,bcol,bline;
	int height = 0, width = 0;
	cct_getconsoleborder(col, line, bcol, bline);
	while(1){
		int option = menu("int", ops, sizeof(ops) / 100);
		switch (option)
		{
			case 1:
				print_interface(height,width,option);
				break;
			case 2:
			case 3:
			case 4:
				testOp(option);
				break;
			case 5:
			case 6:
				tetris(option);
				break;
			case 0:
				return 0;
		}
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
		cct_setconsoleborder(col, line, bcol, bline);
	}

	return 0;
}
