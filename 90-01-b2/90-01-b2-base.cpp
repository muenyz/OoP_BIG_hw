/* 2253731 计科 张皓钦 */
#include "90-01-b2.h"
using namespace std;

void set_size(int& row, int& column)
{
	while (1) {
		cout << "请输入行数(8-10)：" << endl;
		cin >> row;
		if (cin.good() && row >= 8 && row <= 10)
			break;
		else {
			cin.clear();
			cin.ignore(32768, '\n');
		}
	}
	while (1) {
		cout << "请输入列数(8-10)：" << endl;
		cin >> column;
		if (cin.good() && column >= 8 && column <= 10)
			break;
		else {
			cin.clear();
			cin.ignore(32768, '\n');
		}
	}
}

void getrc(int c_max,int r_max,char a[])
{
	while(1){
		cin.clear();
		cin.ignore(7355608,'\n');
		cin.clear();
		cin.get(a, 3, '\n');
		if (a[0] >= 'a' && a[0] <= 'z')
			a[0] -= 32;
		if (a[0] - 'A' + 1 > r_max || a[0] < 'A' || a[1]<'0' || a[1] - '0' + 1>c_max)
			print_reminder("wrong");
		else
			break;
	}
	cout << "输入为" << a[0] << "行" << a[1] << "列" << endl;
}

void set_initial_state(int row, int column, int a[][N + 2])
{
	srand((unsigned int)time(0));
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= column; j++)
			a[i][j] = rand() % 5 + 1;
}

void print_state(int row, int column, int a[][N + 2],char b[][N+2],int po,int display,int d1oO)
{
	cout << "  |";
	for (int i = 0; i < column; i++)
		cout << "  " << i;
	cout << endl;
	cout << "--+";
	for (int i = 0; i < column; i++)
		cout << "---";
	cout << endl;
	for (int i = 1; i <= row; i++) {
		cout << char(i - 1 + 'A') << " |";
		for (int j = 1; j <= column; j++) {
			cout << "  ";
			if (po == 1)
				cout << (b[i][j] ? b[i][j] : '0');
			else {
				if (display) {
					if (d1oO ? b[i][j] == '*' : a[i][j] == 0 || b[i][j] == '*')
						cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
				}
				cout << a[i][j];
				cct_setcolor();
			}
		}
		cout << endl;
	}
}

int match_stars(int a[][N + 2], char b[][N + 2], int r, int c)
{
	int i = 1;
	b[r][c] = '*';
	if (a[r][c] == a[r + 1][c] && b[r + 1][c] != '*')
		i += match_stars(a, b, r + 1, c);
	if (a[r][c] == a[r][c + 1] && b[r][c + 1] != '*')
		i += match_stars(a, b, r, c + 1);
	if (a[r][c] == a[r - 1][c] && b[r - 1][c] != '*')
		i += match_stars(a, b, r - 1, c);
	if (a[r][c] == a[r][c - 1] && b[r][c - 1] != '*')
		i += match_stars(a, b, r, c - 1);
	return i;
}

char get_confirm()
{
	int x, y;
	char a;
	cct_getxy(x, y);
	while (a = _getch()) {
		if (a >= 'a' && a <= 'z')
			a -= 32;
		if (a == 'Y' || a == 'N' || a == 'Q'){
			putchar(a);
			cout << endl;
			break;
		}
	}
	return a;
}

void move_insides(int row,int column,int a[][N + 2], char b[][N + 2],int mode,const location loc[][N+2],int frame)
{
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= column; j++) {
			if (b[i][j] == '*')
				a[i][j] = 0;
		}
	}
	if (mode == 0)
		return;
	else {
		for (int i = 1; i <= row; i++) {
			for (int j = 1; j <= column; j++) {
				if (b[i][j] == '*') {
					for (int k = i; a[k-1][j] != 0; k--) {
						if(mode==2)
							move_a_star(loc, k - 1, j, a[k - 1][j], 0, frame);
						b[k][j] = 0;
						b[k-1][j] = '*';
						int tmp = a[k][j];
						a[k][j] = a[k - 1][j];
						a[k - 1][j] = tmp;
					}
				}
			}
		}
		for (int j = column-1; j>=1; j--) {
			if (b[row][j] == '*') {
				for(int k=j;a[row][k+1]!=0;k++){
					char tmp;
					for (int i = 1; i <= row; i++) {
						if (mode == 2)
							move_a_star(loc, i, k+1, a[i][k+1], 1, frame);
						tmp = b[i][k];
						b[i][k] = b[i][k + 1];
						b[i][k + 1] = tmp;
						tmp = a[i][k];
						a[i][k] = a[i][k + 1];
						a[i][k + 1] = tmp;
					}
				}
			}
		}
	}
}

int end_judge(int row, int column, int a[][N + 2])
{
	int count = 0;
	for(int i=0;i<=row;i++)
		for(int j=0;j<=column;j++){
			if(a[i][j]){
				char b[N + 2][N + 2] = { 0 };
				if (match_stars(a, b, i, j) != 1)
					return 0;
				else
					count++;
			}
		}
	return count;
}

void inside(int row, int column, int a[][N + 2],char rc[],const int option)
{
	int RANK=0, rank,count=0;
	cout << endl;
	do{
		if (end_judge(row, column, a)){
			cout << "剩余" << end_judge(row, column, a) << "个不可消除项，本关结束!" << endl;
			break;
		}
		char b[N + 2][N + 2] = { 0 };
		cout << endl;
		cout << "当前数组：" << endl;
		print_state(row, column, a, b, 0, 1,0);
		while (1) {
			int r, c;
			print_reminder("getrc");
			getrc(column, row, rc);
			c = rc[1] - '0' + 1;
			r = rc[0] - 'A' + 1;
			if (a[r][c] == 0) {
				print_reminder("illegal");
				continue;
			}
			if ((rank = match_stars(a, b, r, c)) == 1) {
				print_reminder("no_match");
				b[r][c] = 0;
			}
			else {
				cout << endl;
				cout << "查找结果数组：" << endl;
				print_state(row, column, a, b, 1, 0,0);
				cout << endl;
				cout << "当前数组(不同色标识)：" << endl;
				print_state(row, column, a, b, 0, 1,1);
				if (option >= 'B') {
					cout << endl;
					cout << "请确认是否把" << rc[0] << rc[1] << "及周围的相同值消除(Y/N/Q)：";
					switch (get_confirm()) {
						case 'Y':
							count += rank;
							rank = rank * rank * 5;
							RANK += rank;
							cout << endl;
							cout << "相同值归并后的数组(不同色标识)：" << endl;
							move_insides(row, column, a, b, 0);
							print_state(row, column, a, b, 0, 1,1);
							cout << endl;
							cout << "本次得分：" << rank << " 总得分：" << RANK << endl;
							cout << endl;
							print_reminder("enter");
							wait_for_enter();
							move_insides(row, column, a, b, 1);
							cout << "下落后的数组：" << endl;
							print_state(row, column, a, b, 0, 1,0);
							cout << endl;
							if (option == 'C') {
								print_reminder("once_end");
								wait_for_enter();
							}
							break;
						case 'N':
							break;
						case 'Q':
							getend();
							return;
					}
					cout << endl;
				}
				break;
			}
		}
	} while (option == 'C' && count != row * column);
	getend();
}
