/*2253731 ¼Æ¿Æ ÕÅğ©ÇÕ*/
#include <iostream>
#include <conio.h>
using namespace std;

void wait_for_enter(const char* reminder)
{
	if (reminder)
		cout << reminder;
	while (_getch() != '\r')
		;
}

int getInt(const char* reminder,int min,int max)
{
	int inputNum;
	while(1){
		if (reminder)
			cout << reminder;
		cin >> inputNum;
		if (inputNum >= min && inputNum <= max){
			cin.ignore(233333, '\n');
			return inputNum;
		}
		else {
			cin.clear();
			cin.ignore(233333, '\n');
		}
	}
}

char getDrctnKey()
{
	char ch = _getch();
	if (ch != -32)
		return 0;
	else
		return ch = _getch();
}