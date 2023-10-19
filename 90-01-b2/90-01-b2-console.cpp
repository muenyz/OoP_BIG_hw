/* 2253731 计科 张皓钦 */
#include "90-01-b2.h"
#include "../include/pattern.h"
using namespace std;
const int x_0 = 0, y_0 = 1;

struct matrix {
	char mx[N + 2][N + 2];
};

const matrix b0 = { {0} };

void map(int row,int column,location b[][N + 2], bool frame)
{
	b[0][1].x = x_0 + 6;
	b[1][0].y = y_0 + 3;
	for (int i = 1; i <= column; i++) {
		if (i > 1)
			b[0][i].x = b[0][i - 1].x + 6 + 2 * frame;
		b[0][i].y = y_0;
	}
	for (int i = 1; i <= row; i++) {
		if (i > 1)
			b[i][0].y = b[i-1][0].y + 3 + frame;
		b[i][0].x = x_0;
	}
	for(int i=1;i<=row;i++)
		for (int j = 1; j <= column; j++) {
			b[i][j].x = b[0][j].x;
			b[i][j].y = b[i][0].y;
		}
}

void set_border(int row, int column, bool frame)
{
	int ccol = 0, cline = 0, cbcol = 0, cbline = 0;
	int brow = 1 + 1 + 3 * row + frame * (row - 1) + 1 +6 , bcol = 2 + 4 + 6 * column + 2 * frame * (column - 1)+1;
	cct_setconsoleborder(bcol, brow);
	cct_getconsoleborder(ccol, cline, cbcol, cbline);
	cct_showstr(0, 0, "", 0, 7, 1, ccol);
	cct_gotoxy(0, 0);
	cout << "屏幕当前设置为：" << brow << "行" << bcol << "列" << endl;
}

void move_a_star(const location b[][N+2],int r, int c, int color, int direction, bool frame)//0下落1平移
{
	if (!color)
		return;
	if (direction == 0) {
		for (int count = 0; count < 3 + frame;count++) {
			print_star(b[r][c].x, b[r][c].y+count, 7, 0);
			print_star(b[r][c].x, b[r][c].y + count+1,color,0);
			Sleep(5);
		}
		if (frame)
			cct_showstr(b[r][c].x - 2, b[r][c].y + 2, "═══", 7, 0);
	}
	else if(direction==1) {
		for (int count = 0; count < 6 + 2*frame; count++) {
			print_star(b[r][c].x-count, b[r][c].y, 7, 0);
			print_star(b[r][c].x-count-1, b[r][c].y, color, 0);
			Sleep(5);
		}
		if (frame)
			for (int k = -1; k <= 1; k++) {
				cct_showstr(b[r][c].x - 4, b[r][c].y + k, "║", 7, 0);
			}
	}
	cct_setcolor();
}

void print_frame(int row, int column, location b[][N+2],bool inop)
{
	if (inop) {
		for (int i = 1; i <= column; i++)
			for (int j = 1; j <= row; j++) {
				cct_showstr(b[j][i].x - 2, b[j][i].y + 2, "═══", 7, 0);
				Sleep(10);
				for (int k = -1; k <= 1; k++) {
					cct_showstr(b[j][i].x + 4, b[j][i].y + k, "║", 7, 0);
				}
				cct_showstr(b[j][i].x + 4, b[j][i].y + 2, "╬", 7, 0);
				Sleep(10);
			}
	}
	for (int i = 1; i <= column; i++)
	{
		cct_showch(b[0][i].x, b[0][i].y, i + '0' - 1, 0, 7);
		cct_showstr(b[0][i].x - 2, b[0][i].y + 1, "═══", 7, 0);
		cct_showstr(b[row][i].x - 2, b[row][i].y + 2, "═══", 7, 0);
		Sleep(10);
		if (inop) {
			cct_showstr(b[0][i].x + 4, b[0][i].y + 1, "╦", 7, 0);
			cct_showstr(b[row][i].x + 4, b[row][i].y + 2, "╩", 7, 0);
		}
	}
	for (int i = 1; i <= row; i++)
	{
		cct_showch(b[i][0].x, b[i][0].y, i - 1 + 'A', 0, 7);
		for (int j = -1; j <= 1; j++) {
			cct_showstr(b[i][0].x + 2, b[i][0].y + j, "║", 7, 0);
			cct_showstr(b[i][column].x + 4, b[i][column].y + j, "║", 7, 0);
			Sleep(10);
		}
		if (inop) {
			cct_showstr(b[i][0].x + 2, b[i][0].y + 2, "╠", 7, 0);
			cct_showstr(b[i][column].x + 4, b[i][column].y + 2, "╣", 7, 0);
		}
		Sleep(10);
	}
	cct_showstr(b[0][1].x - 4, b[0][1].y + 1, "╔", 7, 0);
	cct_showstr(b[row][1].x - 4, b[row][1].y + 2, "╚", 7, 0);
	cct_showstr(b[0][column].x + 4, b[0][column].y + 1, "╗", 7, 0);
	cct_showstr(b[row][column].x + 4, b[row][column].y + 2, "╝", 7, 0);
	cct_setcolor();
}

int choose_star(int& r, int& c, int row, int column, int a[][N + 2], location loc[][N + 2], char option, matrix& b,int&RANK,int&rank)
{
	int ccol = 0, cline = 0, cbcol = 0, cbline = 0;
	bool illegal_flag = 0;
	bool choose_flag = 0;
	while (1) {
		int MX = -1, MY = -1, MACTION = 0, key_code1 = 0, key_code2 = 0;
		switch (cct_read_keyboard_and_mouse(MX, MY, MACTION, key_code1, key_code2)) {
			case CCT_KEYBOARD_EVENT:
				if (key_code1 == 0xe0) {
					print_star(loc[r][c].x, loc[r][c].y, a[r][c], 0);
					for (int i = 1; i <= row; i++)
						for (int j = 1; j <= column; j++)
							if (b.mx[i][j] == '*')
								print_star(loc[i][j].x, loc[i][j].y, a[i][j], 0);
					switch (key_code2) {
						case KB_ARROW_UP:
							while (1)
							{
								r--;
								if (r == 0)
									r = row;
								if (a[r][c])
									break;
							}
							break;
						case KB_ARROW_DOWN:
							while (1)
							{
								r++;
								if (r > row)
									r = 1;
								if (a[r][c])
									break;
							}
							break;
						case KB_ARROW_LEFT:
							while (1)
							{
								c--;
								if (c == 0)
									c = column;
								if (a[r][c])
									break;
							}
							break;
						case KB_ARROW_RIGHT:
						while(1){
							c++;
							if (c > column)
								c = 1;
							if (a[r][c])
								break;
						}
							break;
					}
					print_star(loc[r][c].x, loc[r][c].y, a[r][c], 7);
					cct_getconsoleborder(ccol, cline, cbcol, cbline);
					cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
					cct_gotoxy(0, loc[row][0].y + 3);
					cout << "[当前键盘] " << char(r - 1 + 'A') << "行" << c - 1 << "列" << endl;
				}
				else if (key_code1 == 'Q' || key_code1 == 'q') {
					if (option == 'D' || option == 'E') {
						cct_getconsoleborder(ccol, cline, cbcol, cbline);
						cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
						cct_gotoxy(0, loc[row][0].y + 3);
						cout << "选中了" << char(r - 1 + 'A') << "行" << c - 1 << "列" << endl;
					}
					return 1;
				}
				else if (key_code1 == '\r') {
					if (choose_flag) {
						for (int i = 1; i <= row; i++)
							for (int j = 1; j <= column; j++)
								if (b.mx[i][j] == '*')
									print_star(loc[i][j].x, loc[i][j].y, 7, 0);
						if (option == 'F') {
							cct_getconsoleborder(ccol, cline, cbcol, cbline);
							cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
							cct_gotoxy(0, loc[row][0].y + 3);
							cout << "合成完成，回车键/单击左键下落" ;
						}
						rank = rank * rank*5;
						RANK += rank;
						return 2;
					}
					if (option == 'D' || option == 'E') {
						cct_getconsoleborder(ccol, cline, cbcol, cbline);
						cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
						cct_gotoxy(0, loc[row][0].y + 3);
						cout << "选中了" << char(r - 1 + 'A') << "行" << c - 1 << "列" << endl;
						return 1;
					}
					else if (option == 'F' || option == 'G') {
						rank = match_stars(a, b.mx, r, c);
						if (rank == 1) {
							cct_getconsoleborder(ccol, cline, cbcol, cbline);
							cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
							cct_showstr(0, loc[row][0].y + 3, "周围无相同值", 0, COLOR_YELLOW);
							cct_setcolor();
							continue;
						}
						else {
							choose_flag = 1;
							for (int i = 1; i <= row; i++)
								for (int j = 1; j <= column; j++) {
									if (b.mx[i][j] == '*') {
										print_star(loc[i][j].x, loc[i][j].y, a[i][j], 7);
									}
								}
							cct_getconsoleborder(ccol, cline, cbcol, cbline);
							cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
							cct_gotoxy(0, loc[row][0].y + 3);
							cout << "箭头键/鼠标移动取消当前选择，回车键/单击左键合成";
							continue;
						}
					}
				}
				else
					continue;
				break;
			case CCT_MOUSE_EVENT:
				bool legal_flag = 0;
				switch (MACTION) {
					case MOUSE_ONLY_MOVED:
						if (!illegal_flag && (MX - loc[r][c].x <= 3 && MX - loc[r][c].x > -3 && fabs(MY - loc[r][c].y) <= 1))
							continue;
						else
						{
							print_star(loc[r][c].x, loc[r][c].y, a[r][c], 0);
							for (int i = 1; i <= row; i++)
								for (int j = 1; j <= column; j++)
									if (b.mx[i][j] == '*')
										print_star(loc[i][j].x, loc[i][j].y, a[i][j], 0);
						}
						for (int i = 1; i <= column; i++) {
							if (MX - loc[0][i].x <= 3 && MX - loc[0][i].x > -3) {
								for (int j = 1; j <= row; j++) {
									if (fabs(MY - loc[j][0].y) <= 1&&a[j][i]) {
										r = j;
										c = i;
										illegal_flag = 0;
										legal_flag = 1;
										print_star(loc[r][c].x, loc[r][c].y, a[r][c], 7);
										cct_getconsoleborder(ccol, cline, cbcol, cbline);
										cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
										cct_gotoxy(0, loc[row][0].y + 3);
										cout << "[当前鼠标] " << char(r - 1 + 'A') << "行" << c - 1 << "列" << endl;
										break;
									}
								}
								break;
							}
						}
						if (!legal_flag) {
							illegal_flag = 1;
							cct_getconsoleborder(ccol, cline, cbcol, cbline);
							cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
							cct_gotoxy(0, loc[row][0].y + 3);
							cout << "[当前鼠标] 位置非法" ;
							continue;
						}
						break;
					case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:
					case MOUSE_LEFT_BUTTON_CLICK:
						if (illegal_flag) {
							continue;
						}
						if (choose_flag) {
							for (int i = 1; i <= row; i++)
								for (int j = 1; j <= column; j++)
									if (b.mx[i][j] == '*')
										print_star(loc[i][j].x, loc[i][j].y, 7, 0);
							if (option == 'F') {
								cct_getconsoleborder(ccol, cline, cbcol, cbline);
								cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
								cct_gotoxy(0, loc[row][0].y + 3);
								cout << "合成完成，回车键/单击左键下落";
							}
							rank = rank * rank * 5;
							RANK += rank;
							return 2;
						}
						if (option == 'D' || option == 'E') {
							cct_getconsoleborder(ccol, cline, cbcol, cbline);
							cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
							cct_gotoxy(0, loc[row][0].y + 3);
							cout << "选中了" << char(r - 1 + 'A') << "行" << c - 1 << "列" << endl;
							return 1;
						}
						else if (option == 'F' || option == 'G') {
							rank = match_stars(a, b.mx, r, c);
							if (rank == 1) {
								cct_showstr(0, loc[row][0].y + 3, "周围无相同值", 0, COLOR_YELLOW, 1, 80);
								cct_setcolor();
								continue;
							}
							else {
								choose_flag = 1;
								for (int i = 1; i <= row; i++)
									for (int j = 1; j <= column; j++) {
										if (b.mx[i][j] == '*') {
											print_star(loc[i][j].x, loc[i][j].y, a[i][j], 7);
										}
									}
								cct_getconsoleborder(ccol, cline, cbcol, cbline);
								cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
								cct_gotoxy(0, loc[row][0].y + 3);
								cout << "箭头键/鼠标移动取消当前选择，回车键/单击左键合成";
								continue;
							}
							break;
						}
						break;
					case MOUSE_RIGHT_BUTTON_CLICK:
					case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:
						if (illegal_flag) {
							continue;
						}
						if (option == 'D' || option == 'E') {
							cct_getconsoleborder(ccol, cline, cbcol, cbline);
							cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
							cct_gotoxy(0, loc[row][0].y + 3);
							cout << "选中了" << char(r - 1 + 'A') << "行" << c - 1 << "列" << endl;
						}
						return 1;
				}
		}
		b = b0;
		choose_flag = 0;
	}
}

int resetrc(int row, int column, int a[][N + 2], int& r, int& c)
{
	for(int i=1;i<=column;i++)
		for (int j = 1; j <= row; j++) {
			if (a[j][i]) {
				r = j;
				c = i;
				return 1;
			}
		}
	return 0;
}

int popstar_interface(int row,int column,int a[][N+2],char option)
{
	int ccol = 0, cline = 0, cbcol = 0, cbline = 0;
	int RANK = 0, rank = 0;
	bool frame=0;
	if (option == 'D' || option == 'F')
		frame = 0;
	else if (option == 'E' || option=='G')
		frame = 1;
	set_border(row, column, frame);
	
	location loc[N + 2][N + 2] = { 0 };

	map(row, column, loc, frame);
	print_frame(row, column, loc, frame);
	for (int i = 1; i <= column; i++)
		for (int j = 1; j <= row; j++)
			print_star(loc[j][i].x, loc[j][i].y, a[j][i], 0);
	cct_setcolor();

	int r = 1, c = 1,r0=r,c0=c;
	cct_setcursor(CURSOR_INVISIBLE);
	do{
		int rest = end_judge(row, column, a);
		if (rest||!resetrc(row,column,a,r,c)) {
			cct_getconsoleborder(ccol, cline, cbcol, cbline);
			cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
			cct_gotoxy(0, loc[row][0].y + 3);
			cout << "剩余" << rest << "个不可消除项，本关结束!按回车进入下一关";
			if (rest < 10) {
				rank = (10 - rest) * 180;
				RANK += rank;
			}
			else
				rank = 0;
			cct_getconsoleborder(ccol, cline, cbcol, cbline);
			cct_showstr(0, 0, "", 0, 7, 1, ccol);
			cct_gotoxy(0, 0);
			cout << "奖励得分：" << rank << " 总得分：" << RANK;
			wait_for_enter();
			return 1;
		}
		resetrc(row, column, a, r, c);
		matrix b = b0;
		print_star(loc[r][c].x, loc[r][c].y, a[r][c], 7);
		cct_getconsoleborder(ccol, cline, cbcol, cbline);
		cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
		cct_gotoxy(0, loc[row][0].y + 3);
		cout << setfill(' ') << "箭头键/鼠标移动，回车键/单击左键选择" << (option <= 'E' ? "并结束" : "，Q/q键或单击右键退出") << endl;
		int ret=choose_star(r, c, row, column, a, loc, option,b,RANK,rank);
		if(ret==2){
			if (option == 'F')
				getkbmz('\r', MOUSE_LEFT_BUTTON_CLICK);
			move_insides(row, column, a, b.mx, 2, loc, frame);
			if (option == 'F') {
				cct_getconsoleborder(ccol, cline, cbcol, cbline);
				cct_showstr(0, loc[row][0].y + 3, "", 0, 7, 1, ccol);
				cct_gotoxy(0, loc[row][0].y + 3);
				cout << "本次合成结束，按C/单击左键继续新一次的合成";
				getkbmz('C', MOUSE_LEFT_BUTTON_CLICK);
				cct_getconsoleborder(ccol, cline, cbcol, cbline);
				cct_showstr(0, loc[row][0].y + 4, "", 0, 7, 1, ccol);
				cct_gotoxy(0, loc[row][0].y + 4);
				cct_setcursor(CURSOR_VISIBLE_NORMAL);
				getend();
				return 0;
			}
			else if (option == 'G')
			{
				cct_getconsoleborder(ccol, cline, cbcol, cbline);
				cct_showstr(0,0, "", 0, 7, 1, ccol);
				cct_gotoxy(0, 0);
				cout << "本次得分：" << rank << " 总得分：" << RANK ;
			}
		}
		else if (ret == 1) {
			cct_getconsoleborder(ccol, cline, cbcol, cbline);
			cct_showstr(0, loc[row][0].y + 4, "", 0, 7, 1, ccol);
			cct_gotoxy(0, loc[row][0].y + 4);
			cct_setcursor(CURSOR_VISIBLE_NORMAL);
			getend();
			return 0;
		}
	} while (option =='G');
	return 0;
}