/*2253731 计科 张皓钦*/
#include "90-02-b1.h"

const int x_0 = 4, y_0 = 2,minHeight=18,maxHeight=26,minWidth=12,maxWidth=21;
const bool numF[10][5][3] = { 
	{1,1,1,1,0,1,1,0,1,1,0,1,1,1,1},
	{0,1,0,0,1,0,0,1,0,0,1,0,0,1,0},
	{1,1,1,0,0,1,1,1,1,1,0,0,1,1,1},
	{1,1,1,0,0,1,1,1,1,0,0,1,1,1,1},
	{1,0,1,1,0,1,1,1,1,0,0,1,0,0,1},
	{1,1,1,1,0,0,1,1,1,0,0,1,1,1,1},
	{1,1,1,1,0,0,1,1,1,1,0,1,1,1,1},
	{1,1,1,0,0,1,0,0,1,0,0,1,0,0,1},
	{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1},
	{1,1,1,1,0,1,1,1,1,0,0,1,1,1,1}
};

struct location {
	int x, y;
};

void map(int row, int column, location b[maxHeight+2][maxWidth+2])
{
	b[0][0].x = x_0;
	b[0][0].y = y_0;
	for (int i = 1; i <= column; i++) {
		b[0][i].x = b[0][i - 1].x + 6;
		b[0][i].y = y_0;
	}
	for (int i = 1; i <= row; i++) {
		b[i][0].y = b[i - 1][0].y + 3;
		b[i][0].x = x_0;
	}
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= column; j++) {
			b[i][j].x = b[0][j].x;
			b[i][j].y = b[i][0].y;
		}
}

struct NumPattern {
	int num, x, y, bg_color, fg_color, state[5][5] = { 0 };
	double speed;
	NumPattern(int num, int x, int y, int bg_color=COLOR_HBLUE, int fg_color=COLOR_BLACK,double setSpeed=1) :num(num), x(x), y(y), bg_color(bg_color), fg_color(fg_color) ,speed(setSpeed)
	{
		for (int i = 0; i < 5; i++) {
			for (int j = 1; j <= 3; j++)
				state[i][j] = numF[num][i][j - 1];
		}
	}
	void print_numP(location b[][maxWidth+2]);
	void erase_numP(int bbg_color, location b[][maxWidth + 2]);
	void move_numP(int bbg_color, int dx, int dy,bool gameMap[][maxWidth+10], location b[][maxWidth + 2]);
	void rotate_numP(int bbg_color, bool gameMap[][maxWidth+10], location b[][maxWidth + 2]);
	bool judge_bottom(bool gameMap[][maxWidth + 10]);
	void kill(bool gameMap[][maxWidth + 10]);
};

void NumPattern::kill(bool gameMap[][maxWidth + 10])
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (state[i][j])
				gameMap[y + i + 5][x + j + 5] = 1;
		}
	}
}

bool NumPattern::judge_bottom(bool gameMap[][maxWidth + 10])
{
	for (int i = 4; i >= 0; i--) {
		for (int j = 0; j < 5; j++)
			if (state[i][j] && gameMap[y + 5 + 1 + i][x + j + 5])
				return 1;
	}
	return 0;
}


void NumPattern::print_numP(location b[][maxWidth+2])
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if(state[i][j]&&y+i>=0)
				print_star(b[y+i][x+j].x, b[y+i][x+j].y, bg_color, fg_color);
		}
	}
}

void NumPattern::erase_numP(int bbg_color, location b[][maxWidth + 2])
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (state[i][j] && y + i >= 0)
				erase_star(b[y + i][x + j].x, b[y + i][x + j].y, COLOR_WHITE);
		}
	}
}

void NumPattern::move_numP(int bbg_color, int dx, int dy, bool gameMap[][maxWidth+10], location b[][maxWidth + 2])
{
	x += dx;
	y += dy;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++) {
			if (state[i][j] && gameMap[y + i+5][x + j+5]){
				x -= dx;
				y -= dy;
				return;
			}
		}
	x -= dx;
	y -= dy;
	erase_numP(bbg_color, b);
	x += dx;
	y += dy;

	print_numP(b);
}

void NumPattern::rotate_numP(int bbg_color, bool gameMap[][maxWidth+10], location b[][maxWidth + 2])
{
	int tmp[5][5] = { 0 };
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++){
			tmp[i][j] = state[4 - j][i];
			if (tmp[i][j] && gameMap [y+i+5][x+j+5])
				return;
		}
	erase_numP(COLOR_WHITE, b);
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			state[i][j] = tmp[i][j];
	print_numP(b);
}

int get_next_num(const bool new_seed = false, const unsigned int seed = 0)
{
	if (new_seed)
		srand(seed);

	return rand() % 10;
}

void print_interface(int& height,int& width, int option)
{
	cct_cls();
	while (1) {
		cout << "请输入游戏界面的高度（" << minHeight << "-"<<maxHeight<<"）：";
		cin >> height;
		if (cin.fail() || height < minHeight || height>maxHeight) {
			cin.clear();
			cin.ignore(233333, '\n');
			continue;
		}
		else {
			cin.ignore(233333, '\n');
			break;
		}
	}
	while (1) {
		cout << "请输入游戏界面的宽度（" << minWidth << "-" << maxWidth << "且是3的倍数）：";
		cin >> width;
		if (cin.fail() || width < minWidth || width>maxWidth ||width%3) {
			cin.clear();
			cin.ignore(233333, '\n');
			continue;
		}
		else {
			cin.ignore(233333, '\n');
			break;
		}
	}
	cct_setcursor(CURSOR_INVISIBLE);
	if (height > 20 || width > 20)
		cct_setfontsize("新宋体", 12, 6);
	int setHeight=height * 3;
	int setWidth=width * 6;
	zinterface(setHeight, setWidth, 7,0,0,true,COLOR_HYELLOW,true,true,6,20);
	cct_showstr(setWidth+4, 3, "下一个数字：");
	cct_showstr(setWidth+4, 1, "当前所得分：");
	cct_showstr(setWidth+4, 2, "已消除行数：");
	cct_showstr(setWidth + 4, 19, "完整版按键：");
	cct_showstr(setWidth + 4, 20, "↑：旋转");
	cct_showstr(setWidth + 4, 21, "↓：立即下落一格");
	cct_showstr(setWidth + 4, 22, "←：左移一格");
	cct_showstr(setWidth + 4, 23, "→：右移一格");
	drawUpperChar('N', setWidth + 4, 24+5,0,COLOR_HYELLOW);
	drawUpperChar('U', setWidth + 4+6+1, 24+5,0,COLOR_HGREEN);
	drawUpperChar('M', setWidth + 4+12+2, 24+5,0, COLOR_HRED);
	drawUpperChar('T', setWidth + 4, 30+5, 0, COLOR_HPINK);
	drawUpperChar('E', setWidth + 4+1+6, 30+5, 0, COLOR_HBLUE);
	drawUpperChar('T', setWidth + 4+2+12, 30+5, 0, COLOR_HWHITE);
	drawUpperChar('R', setWidth + 4, 36+5, 0, COLOR_HCYAN);
	drawUpperChar('I', setWidth + 4+1+6, 36+5, 0, COLOR_GREEN);
	drawUpperChar('S', setWidth + 4 + 2+12, 36+5, 0, COLOR_RED);
	if(option){
		cct_gotoxy(setWidth+4, setHeight);
		wait_for_enter("按回车键继续...");
		cct_setfontsize("新宋体", 16, 8);
	}
}

int clear(int height, int width, bool gameMap[][maxWidth + 10], location b[][maxWidth + 2])
{
	int cnt = 0;
	for (int i = height - 1; i >= 0;) {
		bool flag = 1;
		for (int j = 0; j < width; j++) {
			flag = flag && gameMap[i + 5][j + 5];
		}
		if (flag) {
			cnt++;
			for (int k = i + 5; k > 0; k--) {
				for (int j = 0; j < width; j++) {
					if (gameMap[k][j + 5] == gameMap[k - 1][j + 5])
						continue;
					if (k - 5 >= 0)
						erase_star(b[k - 5][j].x, b[k - 5][j].y);
					if ((gameMap[k][j + 5] = gameMap[k - 1][j + 5]) && k - 5 >= 0)
						print_star(b[k - 5][j].x, b[k - 5][j].y, COLOR_HBLUE);
				}
			}
		}
		else
			i--;
	}
	return cnt;
}

void print_num(int num,int x,int y)
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 3; j++)
		{
			erase_star(x + 6 * j + 2, y + i * 3 + 1, COLOR_BLACK);
			if (numF[num][i][j])
				print_star(x + 6 * j + 2, y + i * 3 + 1, COLOR_HBLUE);
		}
}

void testOp(int option)
{
	int num,height=0,width=0;
	location b[maxHeight + 2][maxWidth + 2];
	bool gameMap[maxHeight + 10][maxWidth + 10];//(5,0)到(4+width,4+height)是合法的
	for (int i = 0; i < maxHeight + 10; i++)
		for (int j = 0; j < maxWidth+10; j++)
			gameMap[i][j] = 1;
	cct_cls();
	num=getInt("请输入要绘制的数字：", 0, 9);
	print_interface(height, width);
	for (int i = 0; i < 5 + height; i++)
		for (int j = 5; j < 5 + width; j++)
			gameMap[i][j] = 0;
	map(height, width, b);
	NumPattern tst(num, 3, 3);
	if (option == 4) {
		tst.x = (width - 1) / 2 - 2;
		tst.y = -5;
	}
	tst.print_numP(b);
	cct_gotoxy(width*6+4, height*3);
	wait_for_enter("按回车键继续...");
	if (option == 3) {
		for(int i=0;i<4;i++){
			tst.rotate_numP(COLOR_WHITE, gameMap, b);
			wait_for_enter();
		}
	}
	if (option == 4) {
		while (1) {
			bool breakFlag = 0;
			for(int i=0;i<5;i++)
				for (int j = 0; j < 5; j++) {
					if (tst.y + i == height - 1 && tst.state[i][j])
						breakFlag = 1;
				}
			if (breakFlag)
				break;
			tst.move_numP(COLOR_WHITE, 0, 1, gameMap, b);
			wait_for_enter();
		}
	}
	cct_setfontsize("新宋体", 16, 8);
}

void tetris(int option)
{
	int height=0, width=0;
	location b[maxHeight + 2][maxWidth + 2];
	bool gameMap[maxHeight + 10][maxWidth + 10] = { 0 };//(5,0)到(4+width,4+height)是合法的
	print_interface(height, width);
	map(height, width, b);
	for (int i = 0; i < maxHeight+10; i++){
		for (int j = 0; j < maxWidth + 10; j++)
			gameMap[i][j] = 1;
	}
	for (int i = 0; i < height + 5; i++) {
		for (int j = 5; j < width+5; j++)
			gameMap[i][j] = 0;
	}
	if (option == 5) {
		for (int i = 0; i < height;i++) {
			for (int j = (width - 1) / 2-3; j >= 0; j--) {
				print_star(b[i][j].x, b[i][j].y, COLOR_HBLUE, COLOR_BLACK);
				gameMap[i + 5][j + 5] = 1;
			}
			for (int j = (width - 1) / 2+3; j <width; j++) {
				print_star(b[i][j].x, b[i][j].y, COLOR_HBLUE, COLOR_BLACK);
				gameMap[i + 5][j + 5] = 1;
			}
		}
	}
	int nextNum = get_next_num(1, (unsigned int) time(0));
	int point = 0;
	int lineCnt = 0;
	while (1) {
		int clk = GetTickCount();
		NumPattern obj(1, (width - 1) / 2 - 2, -5);
		if (option == 6){
			NumPattern tmp(nextNum, (width - 1) / 2 - 2, -5);
			obj = tmp;
			nextNum = get_next_num();
			if (point >= 120)
				obj.speed = 0.3;
			else
				obj.speed -= point / 15*0.1;
			cct_gotoxy(width * 6 + 12+4, 3);
			cout << setw(8) << nextNum;
			cct_gotoxy(width * 6 + 12+4, 1);
			cout << setw(8) << point;
			cct_gotoxy(width * 6 + 12+4, 2);
			cout << setw(8) << lineCnt;
			print_num(nextNum, width * 6 + 4, 4);
		}
		if (obj.judge_bottom(gameMap))
			break;
		while (1) {
			if (_kbhit()) {
				char ch = getDrctnKey();
				switch (ch) {
					case DRUP:
						obj.rotate_numP(COLOR_WHITE, gameMap, b);
						break;
					case DRDOWN:
						obj.move_numP(COLOR_WHITE, 0, 1, gameMap, b);
						break;
					case DRLEFT:
						obj.move_numP(COLOR_WHITE, -1, 0, gameMap, b);
						break;
					case DRRIGHT:
						obj.move_numP(COLOR_WHITE, 1, 0, gameMap, b);
						break;
					default:
						break;
				}
			}
			if((GetTickCount()-clk)/int(obj.speed*1000)){
				clk = GetTickCount();
				if (obj.judge_bottom(gameMap))
					break;
				obj.move_numP(COLOR_WHITE, 0, 1, gameMap, b);
			}
		}
		obj.kill(gameMap);
		int cLine = clear(height, width, gameMap, b);
		lineCnt += cLine;
		switch (cLine) {
			case 1:
				point += 1;
				break;
			case 2:
				point += 3;
				break;
			case 3:
				point += 6;
				break;
			case 4:
				point += 10;
				break;
			case 5:
				point += 15;
				break;
			case 0:
			default:
				break;
		}
		if (option == 5)
			break;
	}
	if (option == 6)
		cct_showstr(width * 6+4, height * 3 - 1, "游戏结束！！！");
	cct_gotoxy(width * 6+4, height * 3);
	wait_for_enter("按回车键继续");
	cct_setfontsize("新宋体", 16, 8);
	return;
}