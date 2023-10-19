/* 2253731 ����� �ƿ� */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//δ���������²Ŵ���������

/* ���������Ҫ��ͷ�ļ� */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS�д˴����в����ߣ���Ӱ������ʹ��

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */
const int maxHeight = 2003;
struct point {
	int x,y;
};
	/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */
static double getDistance(double x1, double y1, double x2, double y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

//����(x,y)�Ƶ�(point_x,point_y)��תangle�Ƕȣ���ʱ�룩
static void rotatePoint(int prex, int prey, int& nowx, int& nowy, int point_x, int point_y, double angle) {
	nowx = point_x + (int)((prex - point_x) * cos(angle) - (prey - point_y) * sin(angle));
	nowy = point_y + (int)((prex - point_x) * sin(angle) + (prey - point_y) * cos(angle));
}

//дһ�������������,Ҫ����ĵ����ر�����ģ�ѡ��ɨ�����㷨
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
	//��ʼ��pNET
	for (int i = 0; i <maxHeight; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	pAET->next = NULL;

	//����pNET
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

	//��pAET
	for (int i = 0;i <= MAXY-MINY;i++) {
		//�½���һ��ѭ�������Աߵ�x������dx,�������һ������ymax==i+MINY,��ɾ���ñ�
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
		//��������pNET[i]�еı߼���pAET����xΪ����x��ͬʱ��dxΪ�����������
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

		//ɨ�������
		p = pAET->next;
		while (p&&p->next) {
			//for (int x = (int)p->x;x <= (int)p->next->x;x++)
				//hdc_base_point(x, i);
			hdc_base_line((int)p->x, i+MINY, (int)p->next->x, i+MINY);
			p = p->next->next;
		}
	}
	//����pNET��pAET
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
/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
            const int fgcolor��ǰ��ɫ
			const int width  ����Ļ��ȣ�����
			const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
	hdc_release();

	/* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("��������", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //������Ŀ�ȼ��߶�ת��Ϊ�ض��������������!!�ĺ��壺�������8/16�ı��������ж�+1
	cct_cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
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
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
    /* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
	hdc_init(_BgColor_, (_FgColor_  + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* ���ֻ���������demoʱ�������hdc_cls()���һ������ȱ�ߵ����������ʱ����
	   ������ֻ������л������⣬���ߴ���ʱֵ */
	Sleep(30);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
            const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x1,y1)-(x2,y2)֮�仭��һ�����ص������
  ���������const int x1�����x���꣬���Ͻ�Ϊ(0,0)
			const int y1�����y���꣬���Ͻ�Ϊ(0,0)
			const int x2���յ�y���꣬���Ͻ�Ϊ(0,0)
			const int y2���յ�y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
            const int y			��y���꣬���Ͻ�Ϊ(0,0)
			const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
			const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	�������к�����ʵ�֣���������������׼�Ķ�
	1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
	2�����к�����ʵ�ֶ�����hdc_base_point/hdc_base_line���������
	3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
	4�������ʵ������ӵĲ���
	5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ����߶�
  ���������const int x1		������x
            const int y1		������y
			const int x2		���յ��x
			const int y2		���յ��y
			const int thickness	���߶εĴ�ϸ����ȱʡֵ��
			const int RGB_value	���߶ε���ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ���������������û���hdc_point����ϣ��ٶ�����
                  Ҳ������hdc_base_point/hdc_base_line����ϣ��ٶȿ죩
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
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
            const int y1		����1�����y
			const int x2		����2�����x
			const int y2		����2�����y
			const int x3		����3�����x
			const int y3		����3�����y
			bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness	���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
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
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
            const int left_up_y			�����Ͻ�y
			const int width				�����
			const int high				���߶�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
			bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	int loc[4][2];//˳ʱ���¼�Ķ�������
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
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int length			���߳�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
            const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
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
	//��Ϊ�Ƕ���int���360�����Կ��԰����ε������361���������
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
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 360,filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
			const int point_y			��Բ�ĵ�y
			const int radius_a			��ƽ����X��İ뾶
			const int radius_b			��ƽ����Y��İ뾶
			const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			����ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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
