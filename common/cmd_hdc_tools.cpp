/* 2253731 张皓钦 计科 */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//未定义的情况下才打开条件编译

/* 允许添加需要的头文件 */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS中此处会有波浪线，不影响正常使用

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */
const int maxHeight = 2003;
struct point {
	int x,y;
};
	/* 此处允许添加自定义函数，但仅限static，即外部不可见 */
static double getDistance(double x1, double y1, double x2, double y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

//将点(x,y)绕点(point_x,point_y)旋转angle角度（逆时针）
static void rotatePoint(int prex, int prey, int& nowx, int& nowy, int point_x, int point_y, double angle) {
	nowx = point_x + (int)((prex - point_x) * cos(angle) - (prey - point_y) * sin(angle));
	nowy = point_y + (int)((prex - point_x) * sin(angle) + (prey - point_y) * cos(angle));
}

//写一个函数填充多边形,要求传入的点是沿边有序的，选用扫描线算法
static inline void hdc_base_point(const int x, const int y);
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2);
static void fillPolygon(int N,point p[],int RGB_value)
{
	if(RGB_value!=INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	int MAXY = 0, MINY = INT_MAX;
	for (int i = 0;i < N;i++) {
		if (p[i].y > MAXY)
			MAXY = p[i].y;
		if (p[i].y < MINY)
			MINY = p[i].y;
	}
	struct NET {
		int ymax;
		double x;
		double dx;
		NET* next;
	}*pNET[maxHeight],*pAET=new NET;
	//初始化pNET
	for (int i = 0; i <maxHeight; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	pAET->next = NULL;

	//建立pNET
	for (int i = 0;i < N;i++) {
		if (p[i].y < p[(i - 1 + N) % N].y) {
			NET* newEdge = new NET;
			newEdge->ymax = p[(i - 1 + N) % N].y;
			newEdge->x = p[i].x;
			newEdge->dx = double(p[(i - 1 + N) % N].x - p[i].x) / (p[(i - 1 + N) % N].y - p[i].y);
			newEdge->next = pNET[p[i].y-MINY]->next;
			pNET[p[i].y-MINY]->next = newEdge;
		}
		if(p[i].y < p[(i + 1) % N].y){
			NET* newEdge = new NET;
			newEdge->ymax = p[(i + 1) % N].y;
			newEdge->x = p[i].x;
			newEdge->dx = double(p[(i + 1) % N].x - p[i].x) / (p[(i + 1) % N].y - p[i].y);
			newEdge->next = pNET[p[i].y-MINY]->next;
			pNET[p[i].y-MINY]->next = newEdge;
		}
	}

	//搞pAET
	for (int i = 0;i <= MAXY-MINY;i++) {
		//新进入一次循环将活性边的x都加上dx,如果有下一条边且ymax==i+MINY,则删除该边
		NET* p = pAET;
		while (p->next) {
			while (p->next->ymax == i+MINY) {
				NET* temp = p->next;
				p->next = p->next->next;
				delete temp;
				if (p->next==NULL)
					break;
			}
			if (p->next == NULL)
				break;
			p->next->x += p->next->dx;
			p = p->next;
		}
		//接下来将pNET[i]中的边加入pAET，以x为主序，x相同时以dx为次序递增排序
		p = pNET[i];
		while (p->next) {
			NET* temp = p->next;
			p->next = p->next->next;
			NET* q = pAET;
			while (q->next) {
				if (q->next->x > temp->x)
					break;
				else if (q->next->x == temp->x&&q->next->dx > temp->dx)
					break;
				q = q->next;
			}
			temp->next = q->next;
			q->next = temp;
		}

		//扫描线填充
		p = pAET->next;
		while (p&&p->next) {
			//for (int x = (int)p->x;x <= (int)p->next->x;x++)
				//hdc_base_point(x, i);
			hdc_base_line((int)p->x, i+MINY, (int)p->next->x, i+MINY);
			p = p->next->next;
		}
	}
	//销毁pNET和pAET
	for (int i = 0; i < maxHeight; i++) {
		delete pNET[i];
	}
	while (pAET->next) {
		NET* temp = pAET->next;
		pAET->next = pAET->next->next;
		delete temp;
	}
	delete pAET;
}
/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
            const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("点阵字体", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cct_cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
    /* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_  + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
            const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x1,y1)-(x2,y2)之间画出一个像素点的连线
  输入参数：const int x1：起点x坐标，左上角为(0,0)
			const int y1：起点y坐标，左上角为(0,0)
			const int x2：终点y坐标，左上角为(0,0)
			const int y2：终点y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
            const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_base_point/hdc_base_line函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：画线段
  输入参数：const int x1		：起点的x
            const int y1		：起点的y
			const int x2		：终点的x
			const int y2		：终点的y
			const int thickness	：线段的粗细（有缺省值）
			const int RGB_value	：线段的颜色（有缺省值）
  返 回 值：
  说    明：本函数可以用基于hdc_point的组合（速度慢）
                  也可以用hdc_base_point/hdc_base_line的组合（速度快）
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	//hdc_point(x1, y1, thickness, RGB_value);
	//hdc_point(x2, y2, thickness, RGB_value);
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	int dx, dy, oldx = INT_MAX, oldy = INT_MAX;
	if(0){
		int tn = thickness + !thickness;
		double angle = atan2(y2 - y1, x2 - x1)+PI/2;
		for (int r = 0; r < tn; r++) {
			dx = (int)round((r-thickness/2)*cos(angle));
			dy = (int)round((r - thickness / 2) * sin(angle));
			if (dx != oldx || dy != oldy) {
				hdc_base_line(x1 + dx, y1 + dy, x2 + dx, y2 + dy);
				//hdc_base_line(x1 - dx, y1 - dy, x2 - dx, y2 - dy);
				//hdc_base_line(x1 + dx + 1, y1 + dy + 1, x2 + dx - 1, y2 + dy - 1);
				//hdc_base_line(x1 - dx + 1, y1 - dy + 1, x2 - dx - 1, y2 - dy - 1);
				oldx = dx;
				oldy = dy;
			}
		}
	}
	else if(0){
		int R = thickness / 2;
		for (int angle = 0; angle <= 360; angle++) {
			dx = int(R * cos(angle));
			dy = int(R * sin(angle));
			if (dx != oldx || dy != oldy) {
				hdc_base_line(x1 + dx, y1 + dy, x2 + dx, y2 + dy);
				hdc_base_line(x1 - dx, y1 - dy, x2 - dx, y2 - dy);
				oldx = dx;
				oldy = dy;
			}
		}
	}
	else if(0){
		double angle = atan2(y2 - y1, x2 - x1);
		int length = (int)getDistance(x1, y1, x2, y2);
		for (int ds = 0; ds < length; ds++) {
			dx = int(ds * cos(angle));
			dy = int(ds * sin(angle));
			if (dx != oldx || dy != oldy)
				hdc_point(x1 + dx, y1 + dy, thickness, RGB_value);
		}
	}
	else {
		double tn = (double)(thickness + !thickness) / 2;
		double dis = getDistance(x1, y1, x2, y2);
		double angle = atan2(y2 - y1, x2 - x1);
		double nowx, nowy, prex, prey;
		for (int x = int(x1 - tn); x <= x1 + tn; x++) {
			for (int y = int(y1 - tn); y <= y1 + tn; y++) {
				if (getDistance(x, y, x1, y1) <= tn)
					hdc_base_point(x, y);
			}
		}
		prex = x1;
		prey = y1;
		for (int s = 0;s <= dis;s++) {
			nowx=x1+s * cos(angle);
			nowy=y1+s * sin(angle);
			for (int x = int(nowx - tn);x <= int(nowx+tn);x++) {
				for (int y = (int)(nowy - tn);y <= (int)(nowy+tn);y++) {
					if(getDistance(x,y,nowx,nowy)<=tn&&getDistance(x,y,prex,prey)>tn)
						hdc_base_point(x, y);
				}
			}
			prex = nowx;
			prey = nowy;
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
            const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x1, y1, x3, y3, thickness, RGB_value);
	hdc_line(x3, y3, x2, y2, thickness, RGB_value);
	if (filled) {
		point p[3];
		p[0].x = x1;
		p[0].y = y1;
		p[1].x = x2;
		p[1].y = y2;
		p[2].x = x3;
		p[2].y = y3;
		fillPolygon(3, p, RGB_value);
	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
            const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	int loc[4][2];//顺时针记录四顶点坐标
	loc[0][0] = left_up_x;
	loc[0][1] = left_up_y;
	for (int i = 1, len = width; i < 4; i++, len = width + high - len) {
		loc[i][0] = int(loc[i - 1][0] + len * cos(PI * (rotation_angles + 90 * (i - 1)) / 180));
		loc[i][1] = int(loc[i - 1][1] + len * sin(PI * (rotation_angles + 90 * (i - 1)) / 180));
	}
	for (int i = 0; i < 4; i++)
		hdc_line(loc[i][0], loc[i][1], loc[(i + 1) % 4][0], loc[(i + 1) % 4][1], thickness, RGB_value);
	if (filled) {
		point p[4];
		for (int i = 0; i < 4; i++) {
			p[i].x = loc[i][0];
			p[i].y = loc[i][1];
		}
		fillPolygon(4, p, RGB_value);
	}
}
/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及边长，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
            const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	int x1, y1, x2, y2;
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	for (int angle = angle_begin; angle-angle_end&&angle-angle_end-360; angle++) {
		x1 = point_x + (int)(radius * sin(angle * PI / 180));
		y1 = point_y - (int)(radius * cos(angle * PI / 180));
		x2 = point_x + (int)(radius * sin((angle + 1) * PI / 180));
		y2 = point_y - (int)(radius * cos((angle + 1) * PI / 180));
		hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	}
}
/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	int x1, y1, x2, y2;
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	hdc_arc(point_x, point_y, radius, angle_begin, angle_end, thickness, RGB_value);
	if ((angle_end - angle_begin) % 360) {
		x1 = point_x + (int)(radius * sin(angle_begin * PI / 180));
		y1 = point_y - (int)(radius * cos(angle_begin * PI / 180));
		x2 = point_x + (int)(radius * sin(angle_end * PI / 180));
		y2 = point_y - (int)(radius * cos(angle_end * PI / 180));
		hdc_line(point_x, point_y, x1, y1, thickness, RGB_value);
		hdc_line(point_x, point_y, x2, y2, thickness, RGB_value);
	}
	//因为角度用int最多360，所以可以把扇形当成最多361边形来填充
	if (filled) {
		point p[370];
		int cnt=0;
		for (int angle = angle_begin; angle-angle_end&&angle-angle_end-360; angle++,cnt++) {
			p[cnt].x = point_x + (int)(radius * sin(angle * PI / 180));
			p[cnt].y = point_y - (int)(radius * cos(angle * PI / 180));
		}
		if ((angle_begin - angle_end) % 360 != 0) {
			p[cnt].x = point_x + (int)(radius * sin(angle_end * PI / 180));
			p[cnt].y = point_y - (int)(radius * cos(angle_end * PI / 180));
			cnt++;
			p[cnt].x = point_x;
			p[cnt].y = point_y;
			cnt++;
		}
		fillPolygon(cnt, p, RGB_value);
	}
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 360,filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	int x1, y1, x2, y2,cnt=0;
	point p[365] = { 0 };
	rotatePoint(point_x + radius_a, point_y, x1, y1, point_x, point_y, -rotation_angles*PI/180);
	for (int angle = 0; angle <= 360; angle++) {
		x2 = point_x + (int)(radius_a * cos((angle + 1) * PI / 180));
		y2 = point_y - (int)(radius_b * sin((angle + 1) * PI / 180));
		rotatePoint(x2, y2, x2, y2, point_x, point_y, -rotation_angles * PI / 180);
		if(x1!=x2||y1!=y2){
			hdc_line(x1, y1, x2, y2, thickness, RGB_value);
			p[cnt].x = x1;
			p[cnt].y = y1;
			cnt++;
		}
		x1 = x2;
		y1 = y2;
	}
	if (filled) {
		fillPolygon(cnt, p, RGB_value);
	}
}

#endif !HDC_SERIES_BY_TEACHER
