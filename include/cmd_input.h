/*2253731 ¼Æ¿Æ ÕÅð©ÇÕ*/
#pragma once
#include <iostream>
using namespace std;
const char DRUP = 72, DRDOWN = 80, DRLEFT = 75, DRRIGHT = 77;
void wait_for_enter(const char* reminder = NULL);
int getInt(const char* reminder=NULL, int min=INT_MIN, int max=INT_MAX);
char getDrctnKey();