/* 2253731 计科 张皓钦 */
#include "90-01-b2.h"
#include "../include/menu.h"
using namespace std;
const char ops[7][100] = { "命令行找出可消除项并标识",
"命令行完成一次消除（分步骤显示）",
"命令行完成一关（分步骤显示）",
"伪图形界面下用鼠标选择一个色块（无分隔线）",
"伪图形界面下用鼠标选择一个色块（有分隔线）",
"伪图形界面完成一次消除（分步骤）",
"伪图形界面完整版" };

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