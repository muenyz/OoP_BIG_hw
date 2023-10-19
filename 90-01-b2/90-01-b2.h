/* 2253731 ¼Æ¿Æ ÕÅð©ÇÕ */
#pragma once
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <ctime>
#include <string.h>
#include "../include/cmd_console_tools.h"
#include <Windows.h>
#include <cmath>

const int N = 10;

struct location {
	int x;
	int y;
};

void set_size(int& row, int& column);
void set_initial_state(int row, int column, int a[][N + 2]);
void inside(int row, int column, int a[][N + 2],  char rc[], int option);
void print_reminder(const char a[]);
void wait_for_enter();
void getend();
char* tj_strlwr(char* str);
int popstar_interface(int row, int column, int a[][N + 2],char option);
int match_stars(int a[][N + 2], char b[][N + 2], int r, int c);
void move_a_star(const location b[][N + 2], int r, int c, int color, int direction = 0, bool frame = 0);
void move_insides(int row, int column, int a[][N + 2], char b[][N + 2], int mode, const location loc[][N + 2] = { 0 }, int frame = 0);
void getkbmz(char kb = '\r', char mz = MOUSE_LEFT_BUTTON_CLICK);
int end_judge(int row, int column, int a[][N + 2]);