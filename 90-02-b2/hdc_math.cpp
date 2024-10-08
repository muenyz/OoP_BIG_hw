/*2253731 计科 张皓钦*/
#include <iostream>
#include "../include/cmd_hdc_tools.h"
static const double PI = 3.14159;
//画一个四叶草
void hdc_draw_Mathematical_curve_2253731()
{
    //进入函数后执行的第一个语句必须是 hdc_cls();
    hdc_cls();

    //画图过程
    const int point_x = 400,point_y = 400,thickness = 0,RGB_value = 0x00ff00;
    int r = int(300 * abs(sin(2 * 0)) + 150 * abs(sin(4 * 0)));
    int x1 = int(point_x + r * cos(0));
    int y1 = int(point_y + r * sin(0));
    int x2, y2;
    for (int angle = 1;angle <= 360;angle++) {
        r = int(300 * abs(sin(2 * angle*PI/180)) + 150 * abs(sin(4 * angle * PI / 180)));
        x2 = int(point_x + r * cos(angle * PI / 180));
        y2 = int(point_y + r * sin(angle * PI / 180));
        hdc_line(x1, y1, x2, y2, thickness, RGB_value);
        hdc_triangle(point_x, point_y, x1, y1, x2, y2, true, 1, RGB_value);
        x1 = x2;
        y1 = y2;
    }
}

