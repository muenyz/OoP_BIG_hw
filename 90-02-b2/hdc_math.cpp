#include <iostream>
#include "../include/cmd_hdc_tools.h"
static const double PI = 3.14159;
//画一个方程为r=100|sin2θ|+50|sin4θ|的图像
void hdc_draw_Mathematical_curve_2253731(const int point_x, const int point_y, int thickness, int RGB_value)
{
    //进入函数后执行的第一个语句必须是 hdc_cls();
    hdc_cls();

    //画图过程
    int r = int(300 * abs(sin(2 * 0)) + 150 * abs(sin(4 * 0)));
    int x1 = int(point_x + r * cos(0));
    int y1 = int(point_y + r * sin(0));
    int x2, y2;
    for (int angle = 1;angle <= 360;angle++) {
        r = int(300 * abs(sin(2 * angle*PI/180)) + 150 * abs(sin(4 * angle * PI / 180)));
        x2 = int(point_x + r * cos(angle * PI / 180));
        y2 = int(point_y + r * sin(angle * PI / 180));
        hdc_line(x1, y1, x2, y2, thickness, RGB_value);
        x1 = x2;
        y1 = y2;
    }
}

