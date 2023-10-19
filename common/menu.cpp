/* 2253731 计科 张皓钦 */
#include <iostream>
#include "../include/cmd_console_tools.h"
#include <iomanip>
#include <conio.h>
#include <string.h>
using namespace std;

/***************************************************************************
  函数名称：menu
  功    能：打印菜单并返回选项
  输入参数：
  返 回 值：选项
  说    明：
***************************************************************************/
int menu(const char* rtnType,const char options[][100],const int numOfOp,const char separator)
{
	char inputOp,outputOp,quitOp;
	if (!strcmp(rtnType, "char"))
	{
		outputOp = 'A';
		quitOp = 'Q';
	}
	else if (!strcmp(rtnType, "int"))
	{
		outputOp = '1';
		quitOp = '0';
	}
	else
		return -1;
	cct_cls();
	int len = 4;
	for (const char(*p)[100] = options; (p - options) < numOfOp; p++) {
		len = max(len, (int)strlen(*p));
	}
	len += 1;
	cout << setfill(separator) << setw(len) << separator << setfill(' ') << endl;
	for (char i = outputOp; i-outputOp < numOfOp; i++) {
		cout << i << '.' << options[i - outputOp] << endl;
	}
	cout << quitOp <<'.'<<"退出"<< endl;
	cout << setfill(separator) << setw(len) << separator << setfill(' ') << endl;
	cout << "[请选择:]";
	while (1) {
		inputOp = _getch();
		if (!strcmp(rtnType, "char") && inputOp >= 'a' && inputOp <= 'z')
			inputOp -= 32;
		if (inputOp==quitOp||inputOp - outputOp >= 0 && inputOp - outputOp < numOfOp) {
			cout << inputOp<<endl;
			break;
		}
	}
	return outputOp=='1'?inputOp-'0':inputOp;
}