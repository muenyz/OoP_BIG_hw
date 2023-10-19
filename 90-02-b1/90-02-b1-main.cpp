/* 2253731 计科 张皓钦 */
#include "90-02-b1.h"
#include "../include/menu.h"

const char ops[][100] = {
	"绘制游戏界面",
	"绘制一个数字",
	"将一个数字旋转一周",
	"单个数字下落至底部",
	"单个数字下落后消行",
	"数字版俄罗斯方块（完整版）"
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
