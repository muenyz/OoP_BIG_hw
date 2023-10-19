/*2253731 ¼Æ¿Æ ÕÅð©ÇÕ*/
#pragma once

void print_star(int x, int y, int bg_color = 7, int fg_color = 0);
void erase_star(int x, int y, int color = 7);
void zinterface(int height, int width, int color, int x_0 = 0, int y_0 = 0, bool frame = 0, int frameColor = 0, bool board = 0, bool boardRight = 0, int boardHeight = 0, int boardWidth = 0, int boardColor = 0);
void drawUpperChar(char chr, int x, int y, int bg_color=0, int color=7);
//const int bufLen = 10000;

/*struct OffScreen {
	HANDLE outSide;
	HANDLE inSide;
	COORD coord{ 0,0 };
	DWORD bytes;
	char content[bufLen];
	bool work;
	OffScreen()
	{
		bytes = 0;
		inSide = GetStdHandle(STD_OUTPUT_HANDLE);
		outSide = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	}
	void on(bool saveOld = 0)
	{
		work = true;
		if (saveOld) {
			ReadConsoleOutputCharacterA(inSide, content, bufLen, coord, &bytes);
			WriteConsoleOutputCharacterA(outSide, content, bufLen, coord, &bytes);
		}
		SetConsoleActiveScreenBuffer(outSide);
	}
	void flush()
	{
		if (work) {
			ReadConsoleOutputCharacterA(inSide, content, bufLen, coord, &bytes);
			WriteConsoleOutputCharacterA(outSide, content, bufLen, coord, &bytes);
		}
	}
	void off()
	{
		work = false;
		SetConsoleActiveScreenBuffer(inSide);
	}
};*/