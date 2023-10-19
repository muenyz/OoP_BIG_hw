/*2253731 ¼Æ¿Æ ÕÅð©ÇÕ*/
#include "../include/cmd_console_tools.h"
#include <Windows.h>

const int upperChar[26][5][3] = {
	//A
	{1,1,1,
	1,0,1,
	1,1,1,
	1,0,1,
	1,0,1},
	//B
	{1,1,0,
	1,0,1,
	1,1,1,
	1,0,1,
	1,1,0},
	//C
	{0,1,0,
	1,0,1,
	1,0,0,
	1,0,1,
	0,1,0},
	//D
	{1,1,0,
	1,0,1,
	1,0,1,
	1,0,1,
	1,1,0},
	//E
	{1,1,1,
	1,0,0,
	1,1,1,
	1,0,0,
	1,1,1},
	//F
	{1,1,1,
	1,0,0,
	1,1,1,
	1,0,0,
	1,0,0},
	//G
	{0,1,0,
	1,0,1,
	1,0,0,
	1,0,1,
	0,1,1},
	//H
	{1,0,1,
	1,0,1,
	1,1,1,
	1,0,1,
	1,0,1},
	//I
	{1,1,1,
	0,1,0,
	0,1,0,
	0,1,0,
	1,1,1},
	//J
	{0,0,1,
	0,0,1,
	0,0,1,
	1,0,1,
	0,1,0},
	//K
	{1,0,1,
	1,0,1,
	1,1,0,
	1,0,1,
	1,0,1},
	//L
	{1,0,0,
	1,0,0,
	1,0,0,
	1,0,0,
	1,1,1},
	//M
	{1,0,1,
	2,1,3,
	2,1,3,
	2,0,3,
	2,0,3},
	//N
	{1,1,1,
	1,0,1,
	1,0,1,
	1,0,1,
	1,0,1},
	//O
	{0,1,0,
	1,0,1,
	1,0,1,
	1,0,1,
	0,1,0},
	//P
	{1,1,0,
	1,0,1,
	1,1,0,
	1,0,0,
	1,0,0},
	//Q
	{0,1,0,
	1,0,1,
	1,0,1,
	1,1,1,
	0,1,1},
	//R
	{1,1,0,
	1,0,1,
	1,1,0,
	1,0,1,
	1,0,1},
	//S
	{0,1,1,
	1,0,0,
	0,1,0,
	0,0,1,
	1,1,0},
	//T
	{1,1,1,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0},
	//U
	{1,0,1,
	1,0,1,
	1,0,1,
	1,0,1,
	1,1,1},
	//V
	{ 1,0,1,
	1,0,1,
	1,0,1,
	1,0,1,
	0,1,0 },
	//W
	{ 2,0,3,
	2,0,3,
	2,0,3,
	2,1,3,
	1,0,1 },
	//X
	{ 1,0,1,
	1,0,1,
	0,1,0,
	1,0,1,
	1,0,1 },
	//Y
	{ 1,0,1,
	1,0,1,
	0,1,0,
	0,1,0,
	0,1,0 },
	//Z
	{ 1,1,1,
	0,0,1,
	0,1,0,
	1,0,0,
	1,1,1 }
};


void print_star(int x, int y, int bg_color, int fg_color)
{
	if (bg_color == 0)
		return;
	if (bg_color != 7) {
		cct_showstr(x - 2, y - 1, "¨X", bg_color, fg_color);
		cct_showstr(x, y - 1, "¨T", bg_color, fg_color);
		cct_showstr(x + 2, y - 1, "¨[", bg_color, fg_color);
		cct_showstr(x - 2, y, "¨U", bg_color, fg_color);
		cct_showstr(x, y, "¡ï", bg_color, fg_color);
		cct_showstr(x + 2, y, "¨U", bg_color, fg_color);
		cct_showstr(x - 2, y + 1, "¨^", bg_color, fg_color);
		cct_showstr(x, y + 1, "¨T", bg_color, fg_color);
		cct_showstr(x + 2, y + 1, "¨a", bg_color, fg_color);
	}
	else {
		cct_showstr(x - 2, y - 1, "", 7, 0, 1, 6);
		cct_showstr(x - 2, y, "", 7, 0, 1, 6);
		cct_showstr(x - 2, y + 1, "", 7, 0, 1, 6);
	}
	cct_setcolor();
}

void erase_star(int x, int y, int color)
{
	cct_showstr(x - 2, y - 1, "", color, 0, 1, 6);
	cct_showstr(x - 2, y, "", color, 0, 1, 6);
	cct_showstr(x - 2, y + 1, "", color, 0, 1, 6);
	cct_setcolor();
}

void move_star(int x, int y, int bg_color, int fg_color,int dx,int dy,int bbg_color)
{
	erase_star(x, y, bbg_color);
	print_star(x, y, bg_color, fg_color);
}

void zinterface(int height, int width, int color,int x_0,int y_0,bool frame, int frameColor,bool board, bool boardRight, int boardHeight, int boardWidth, int boardColor)
{
	cct_cls();
	cct_setconsoleborder(x_0+width + (frame ? 6 : 0) + (board ? boardWidth : 0), y_0+height + 2 + (frame ? 2 : 0));
	if (frame) {
		int frameLength = (board ? boardWidth + 2 : 0) + width;
		cct_showstr(x_0, y_0, "¨X", color, frameColor);
		cct_showstr(x_0+2+frameLength, y_0,"¨[", color, frameColor);
		cct_showstr(x_0, y_0+1+height, "¨^", color, frameColor);
		cct_showstr(x_0 + 2 + frameLength, y_0 + 1 + height, "¨a", color, frameColor);
		cct_showstr(x_0+2, y_0, "¨T", color, frameColor,frameLength/2);
		cct_showstr(x_0+2, y_0+1+ height, "¨T", color, frameColor, frameLength / 2);
		for (int i = 1; i <= height; i++) {
			cct_showstr(x_0, y_0+i, "¨U", color, frameColor);
			cct_showstr(x_0+2 + frameLength, y_0+i, "¨U", color, frameColor);
		}
		if (board) {
			int putX = (boardRight ? x_0 + 2 + width : x_0 + 2 + boardWidth);
			cct_showstr(putX, y_0, "¨j", color, frameColor);
			cct_showstr(putX, y_0+1+height, "¨m", color, frameColor);
			for (int i = 1; i <= height; i++) {
				cct_showstr(putX, y_0 + i, "¨U", color, frameColor);
			}
		}
	}
	for (int i = frame; i < height+frame; i++) {
		cct_showstr(2*frame+(board && boardRight ? 0 : boardWidth+2*frame), i, "", color, 7, 1, width);
	}
	cct_setcolor();
}

void drawUpperChar(char chr,int x,int y,int bg_color,int color)
{
	if (chr < 'A' || chr>'Z')
		return;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 3; j++)
			cct_showstr(x + 2 * j + (upperChar[chr - 'A'][i][j]==3), y + i, "", upperChar[chr - 'A'][i][j] ? color : bg_color, 7, 1, upperChar[chr - 'A'][i][j] > 1 ? 1 : 2);
	cct_setcolor();
}