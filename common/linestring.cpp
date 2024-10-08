/*2253731 ¼Æ¿Æ ÕÅð©ÇÕ*/
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

void trimLine(string& line, const string& trimType)
{
	if (trimType == "left" || trimType == "all")
		line.erase(0, line.find_first_not_of(" \t\r"));
	if (trimType == "right" || trimType == "all")
		line.erase(line.find_last_not_of(" \t\r") + 1);
}


void printHexLine(const string& line)
{
	const int lineWidth = 51;
	stringstream in;
	in << line;
	char lineBuf[16];
	int lineCount = 0;
	cout << hex;
	do {
		in.read(lineBuf, 16);
		int n = (int)in.gcount();
		if (n == 0)
			break;
		cout << setw(8) << setfill('0') << lineCount << " : ";
		lineCount += 16;
		for (int i = 0; i < n; i++)
		{
			if (i == 8)
				cout << "- ";
			cout << setw(2) << (int)(unsigned char)lineBuf[i] << ' ';
		}
		cout << setfill(' ') << setw(lineWidth - 3 * n - 2 * (n > 8)) << "";
		for (int i = 0; i < n; i++)
		{
			if (lineBuf[i] >= ' ' && lineBuf[i] <= '~')
				cout << lineBuf[i];
			else
				cout << '.';
		}
		cout << endl;
	} while (in.good());
	cout << dec;
}

void cutComment(string& str,const char* const commentChar)
{
	int pos = str.find_first_of(commentChar);
	if (pos != string::npos)
		str.erase(pos);
}

int judgeCcomment(const string& str)
{
	if (str.length() < 2) {
		return 0;
	}
	else if (str.substr(0, 2) == "//" || (str.substr(0, 2) == "/*" && str.substr(str.length() - 2, 2) == "*/")) {
		return 1;
	}
	else if (str.substr(0, 2) == "/*") {
		return 2;
	}
	else {
		return 0;
	}
}
