/* 2253731 �ƿ� ����� */
#pragma once
#include <iostream>
using namespace std;
void trimLine(string& line, const string& trimType);
void printHexLine(const string& line);
void cutComment(string& str, const char* const commentChar = ";#");
int judgeCcomment(const string& str);