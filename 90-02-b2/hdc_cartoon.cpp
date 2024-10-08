/*2253731 计科 张皓钦*/
#include <iostream>
#include "../include/cmd_hdc_tools.h"

static const int defaultBasex = 400, defaultBasey = 400;
void hdc_draw_cartoon_2253731(const int base_x, const int base_y)
{
    //进入函数后执行的第一个语句必须是 hdc_cls();
    hdc_cls();

    //画图过程
    // 脖子
    hdc_circle(446+base_x-defaultBasex, 772+base_y-defaultBasey, 43, 1, 1, 0x00ddf3ff);
    hdc_sector(352 + base_x - defaultBasex, 681 + base_y - defaultBasey, 141, 105, 165, 1, 1, 0x00a7c5f5);
    //头发底色
    hdc_circle(624 + base_x - defaultBasex, 699 + base_y - defaultBasey, 113, 1, 1, 0x0026395a);//右下暗处
    hdc_sector(-330 + base_x - defaultBasex, 324 + base_y - defaultBasey, 1122, 88, 102, 1, 2, 0x004c6492);//右中段扇
    hdc_square(0 + base_x - defaultBasex, 300 + base_y - defaultBasey, 100, 0, 1, 1, 0);
    hdc_sector(569 + base_x - defaultBasex, 374 + base_y - defaultBasey, 566, 250, 285, 1, 1, 0x004c6492);//左中段扇
    hdc_sector(177 + base_x - defaultBasex, 698 + base_y - defaultBasey, 190, 313, 77, 1, 1, 0x004c6492);
    hdc_sector(71 + base_x - defaultBasex, 856 + base_y - defaultBasey, 293, 353, 79, 1, 1, 0x004c6492);
    hdc_sector(1311 + base_x - defaultBasex, 504 + base_y - defaultBasey, 1207, 255, 272, 1, 1, 0x002a3e61);
    hdc_triangle(36 + base_x - defaultBasex, 567 + base_y - defaultBasey, 58 + base_x - defaultBasex, 801 + base_y - defaultBasey, 96 + base_x - defaultBasex, 801 + base_y - defaultBasey, 1, 1, 0);
    hdc_triangle(783 + base_x - defaultBasex, 474 + base_y - defaultBasey, 769 + base_x - defaultBasex, 549 + base_y - defaultBasey, 604 + base_x - defaultBasex, 488 + base_y - defaultBasey, 1, 1, 0x004c6492);
    hdc_triangle(783 + base_x - defaultBasex, 462 + base_y - defaultBasey, 780 + base_x - defaultBasex, 492 + base_y - defaultBasey, 731 + base_x - defaultBasex, 476 + base_y - defaultBasey, 1, 1, 0x004c6492);

    hdc_circle(404 + base_x - defaultBasex, 307 + base_y - defaultBasey, 387, 1, 1, 0x004c6492);//大圆
    hdc_sector(168 + base_x - defaultBasex, 439 + base_y - defaultBasey, 535, 105, 132, 1, 1, 0x004c6492);//右下左段扇
    hdc_triangle(484 + base_x - defaultBasex, 720 + base_y - defaultBasey, 451 + base_x - defaultBasex, 798 + base_y - defaultBasey, 571 + base_x - defaultBasex, 798 + base_y - defaultBasey, 1, 1, 0x004c6492);
    //右下右段修整
    hdc_sector(1171 + base_x - defaultBasex, 690 + base_y - defaultBasey, 502, 257, 281, 1, 1, 0x004c6492);
    hdc_sector(755 + base_x - defaultBasex, 601 + base_y - defaultBasey, 85, 220, 0, 1, 1, 0x004c6492);
    hdc_sector(774 + base_x - defaultBasex, 582 + base_y - defaultBasey, 44, 215, 348, 1, 1, 0x004c6492);
    hdc_sector(737 + base_x - defaultBasex, 810 + base_y - defaultBasey, 59, 280, 52, 1, 1, 0x004c6492);
    hdc_sector(1875 + base_x - defaultBasex, 784 + base_y - defaultBasey, 1133, 269, 283, 1, 1, 0);
    hdc_sector(788 + base_x - defaultBasex, 476 + base_y - defaultBasey, 68, 87, 197, 1, 1, 0);


    //额头
    hdc_sector(508 + base_x - defaultBasex, 346 + base_y - defaultBasey, 369, 297, 330, 1, 1, 0x00ddf3ff);
    hdc_sector(287 + base_x - defaultBasex, 288 + base_y - defaultBasey, 265, 9, 58, 1, 1, 0x00ddf3ff);
    hdc_arc(906 + base_x - defaultBasex, 500 + base_y - defaultBasey, 785, 279, 294, 22, 0x00ddf3ff);
    hdc_arc(273 + base_x - defaultBasex, 299 + base_y - defaultBasey, 268, 58, 97, 30, 0x00ddf3ff);
    hdc_arc(74 + base_x - defaultBasex, 396 + base_y - defaultBasey, 488, 75, 92, 20, 0x00ddf3ff);
    hdc_arc(-13 + base_x - defaultBasex, 396 + base_y - defaultBasey, 585, 73, 98, 1, 0x00ddf3ff);
    hdc_circle(254 + base_x - defaultBasex, 271 + base_y - defaultBasey, 103, 1, 1, 0x00ddf3ff);
    hdc_circle(462 + base_x - defaultBasex, 246 + base_y - defaultBasey, 86, 1, 1, 0x00ddf3ff);


    //脸盘
    hdc_circle(341 + base_x - defaultBasex, 437 + base_y - defaultBasey, 229, 1, 1, 0x00ddf3ff);
    hdc_circle(344 + base_x - defaultBasex, 581 + base_y - defaultBasey, 157, 1, 1, 0x00ddf3ff);

    //眉毛
    hdc_sector(202 + base_x - defaultBasex, 348 + base_y - defaultBasey, 52, 330, 30, 1, 1, 0x008da1b9);
    hdc_sector(201 + base_x - defaultBasex, 628 + base_y - defaultBasey, 326, 356, 4, 1, 1, 0x00ddf3ff);
    hdc_sector(460 + base_x - defaultBasex, 333 + base_y - defaultBasey, 62, 307, 21, 1, 1, 0x008da1b9);
    hdc_sector(492 + base_x - defaultBasex, 445 + base_y - defaultBasey, 169, 331, 360, 1, 1, 0x00ddf3ff);
    //耳朵
    hdc_sector(217 + base_x - defaultBasex, 516 + base_y - defaultBasey, 110, 209, 287, 1, 1, 0x00ddf3ff);

    //下巴
    hdc_sector(268 + base_x - defaultBasex, 600 + base_y - defaultBasey, 104, 205, 263, 1, 1, 0x00ddf3ff);
    hdc_sector(385 + base_x - defaultBasex, 606 + base_y - defaultBasey, 154, 140, 211, 1, 1, 0x00ddf3ff);
    hdc_sector(505 + base_x - defaultBasex, 650 + base_y - defaultBasey, 77, 196, 265, 1, 1, 0x00ddf3ff);
    hdc_arc(19 + base_x - defaultBasex, 427 + base_y - defaultBasey, 539, 100, 114, 6, 0x00ddf3ff);
    hdc_arc(412 + base_x - defaultBasex, 476 + base_y - defaultBasey, 149, 99, 122, 2, 0x00ddf3ff);
    hdc_arc(772 + base_x - defaultBasex, 749 + base_y - defaultBasey, 279, 292, 298, 2, 0x00ddf3ff);
    hdc_arc(-543 + base_x - defaultBasex, 257 + base_y - defaultBasey, 1129, 103, 114, 2, 0x00ddf3ff);
    hdc_arc(626 + base_x - defaultBasex, 42 + base_y - defaultBasey, 762, 205, 212, 10, 0x00ddf3ff);
    hdc_triangle(484 + base_x - defaultBasex, 719 + base_y - defaultBasey, 508 + base_x - defaultBasex, 668 + base_y - defaultBasey, 462 + base_x - defaultBasex, 549 + base_y - defaultBasey, 1, 1, 0x00ddf3ff);

    //右眼
    hdc_sector(239 + base_x - defaultBasex, 592 + base_y - defaultBasey, 89, 310, 19, 1, 1, 0x007488a6);
    hdc_sector(247 + base_x - defaultBasex, 643 + base_y - defaultBasey, 130, 325, 12, 1, 1, 0x00ddf3ff);
    hdc_triangle(260 + base_x - defaultBasex, 503 + base_y - defaultBasey, 261 + base_x - defaultBasex, 511 + base_y - defaultBasey, 278 + base_x - defaultBasex, 516 + base_y - defaultBasey, 1, 1, 0x007488a6);
    //左眼
    hdc_sector(508 + base_x - defaultBasex, 661 + base_y - defaultBasey, 179, 341, 17, 1, 1, 0x007488a6);
    hdc_sector(507 + base_x - defaultBasex, 669 + base_y - defaultBasey, 175, 338, 17, 1, 1, 0x00ddf3ff);
    hdc_triangle(455 + base_x - defaultBasex, 489 + base_y - defaultBasey, 444 + base_x - defaultBasex, 503 + base_y - defaultBasey, 462 + base_x - defaultBasex, 499 + base_y - defaultBasey, 1, 1, 0x007488a6);
    hdc_triangle(455 + base_x - defaultBasex, 501 + base_y - defaultBasey, 472 + base_x - defaultBasex, 497 + base_y - defaultBasey, 457 + base_x - defaultBasex, 491 + base_y - defaultBasey, 1, 1, 0x007488a6);

    //腮红
    hdc_circle(184 + base_x - defaultBasex, 594 + base_y - defaultBasey, 23, 1, 1, 0x00c0d4fe);

    //嘴巴
    hdc_ellipse(377 + base_x - defaultBasex, 655 + base_y - defaultBasey, 62, 33, 3, 1, 1, 0x00a29ee6);
    hdc_arc(376 + base_x - defaultBasex, 705 + base_y - defaultBasey, 80, 320, 44, 10, 0x009696d6);
    hdc_arc(374 + base_x - defaultBasex, 728 + base_y - defaultBasey, 100, 0, 35, 15, 0x009696d6);
    hdc_arc(379 + base_x - defaultBasex, 604 + base_y - defaultBasey, 82, 149, 211, 13, 0x00a6a6ec);
    //鼻子
    hdc_circle(317 + base_x - defaultBasex, 549 + base_y - defaultBasey, 2, 1, 1, 0x008599ad);

    //尾杀
    hdc_triangle(198 + base_x - defaultBasex, 800 + base_y - defaultBasey, 362 + base_x - defaultBasex, 757 + base_y - defaultBasey, 393 + base_x - defaultBasex, 802 + base_y - defaultBasey, 1, 1, 0x002a3e61);
    hdc_triangle(392 + base_x - defaultBasex, 804 + base_y - defaultBasey, 391 + base_x - defaultBasex, 761 + base_y - defaultBasey, 360 + base_x - defaultBasex, 756 + base_y - defaultBasey, 1, 1, 0x002a3e61);
    hdc_triangle(326 + base_x - defaultBasex, 747 + base_y - defaultBasey, 373 + base_x - defaultBasex, 760 + base_y - defaultBasey, 177 + base_x - defaultBasex, 809 + base_y - defaultBasey, 1, 1, 0x002a3e61);
    hdc_triangle(115 + base_x - defaultBasex, 414 + base_y - defaultBasey, 107 + base_x - defaultBasex, 470 + base_y - defaultBasey, 121 + base_x - defaultBasex, 495 + base_y - defaultBasey, 1, 1, 0x00ddf3ff);
    hdc_triangle(120 + base_x - defaultBasex, 387 + base_y - defaultBasey, 108 + base_x - defaultBasex, 467 + base_y - defaultBasey, 133 + base_x - defaultBasex, 515 + base_y - defaultBasey, 1, 1, 0x00ddf3ff);
    hdc_triangle(114 + base_x - defaultBasex, 405 + base_y - defaultBasey, 106 + base_x - defaultBasex, 467 + base_y - defaultBasey, 169 + base_x - defaultBasex, 447 + base_y - defaultBasey, 1, 1, 0x00ddf3ff);
    //轮廓
    // 
    //头发
    hdc_arc(5011 + base_x - defaultBasex, -254 + base_y - defaultBasey, 5030, 258, 259, 1, 0x00000000);
    hdc_arc(-3407 + base_x - defaultBasex, 1638 + base_y - defaultBasey, 3602, 74, 75, 1, 0x00000000);
    hdc_arc(2779 + base_x - defaultBasex, -78 + base_y - defaultBasey, 2818, 255, 257, 1, 0x00000000);
    hdc_arc(935 + base_x - defaultBasex, 313 + base_y - defaultBasey, 935, 254, 261, 1, 0x00000000);
    hdc_arc(521 + base_x - defaultBasex, 366 + base_y - defaultBasey, 518, 259, 285, 1, 0x00000000);
    hdc_arc(410 + base_x - defaultBasex, 311 + base_y - defaultBasey, 397, 282, 323, 1, 0x00000000);
    hdc_arc(421 + base_x - defaultBasex, 300 + base_y - defaultBasey, 370, 35, 84, 1, 0x00000000);
    hdc_arc(172 + base_x - defaultBasex, 346 + base_y - defaultBasey, 623, 82, 101, 1, 0x00000000);
    hdc_arc(-1973 + base_x - defaultBasex, 7 + base_y - defaultBasey, 2794, 100, 101, 1, 0x00000000);
    hdc_arc(1804 + base_x - defaultBasex, 791 + base_y - defaultBasey, 1063, 270, 282, 1, 0x00000000);
    hdc_arc(-230 + base_x - defaultBasex, 815 + base_y - defaultBasey, 366, 75, 87, 1, 0x00000000);
    hdc_arc(1716 + base_x - defaultBasex, 446 + base_y - defaultBasey, 1616, 260, 267, 1, 0x00000000);
    hdc_arc(1001 + base_x - defaultBasex, 541 + base_y - defaultBasey, 899, 272, 294, 1, 0x00000000);
    hdc_arc(483 + base_x - defaultBasex, 322 + base_y - defaultBasey, 337, 295, 332, 1, 0x00000000);
    hdc_arc(275 + base_x - defaultBasex, 305 + base_y - defaultBasey, 284, 10, 82, 1, 0x00000000);
    hdc_arc(64 + base_x - defaultBasex, 394 + base_y - defaultBasey, 509, 75, 94, 1, 0x00000000);
    hdc_arc(262 + base_x - defaultBasex, 426 + base_y - defaultBasey, 309, 91, 108, 1, 0x00000000);
    hdc_arc(-651 + base_x - defaultBasex, 219 + base_y - defaultBasey, 1244, 104, 118, 1, 0x00000000);
    hdc_arc(-226 + base_x - defaultBasex, 259 + base_y - defaultBasey, 965, 87, 111, 1, 0x00000000);
    hdc_arc(63 + base_x - defaultBasex, 384 + base_y - defaultBasey, 651, 109, 129, 1, 0x00000000);
    hdc_arc(1189 + base_x - defaultBasex, 699 + base_y - defaultBasey, 521, 259, 281, 1, 0x00000000);
    hdc_arc(1281 + base_x - defaultBasex, 726 + base_y - defaultBasey, 585, 263, 274, 1, 0x00000000);
    hdc_line(743 + base_x - defaultBasex, 583 + base_y - defaultBasey, 738 + base_x - defaultBasex, 737 + base_y - defaultBasey, 1, 0x00000000);
    //hdc_arc(-2187 + base_x - defaultBasex, 536 + base_y - defaultBasey, 2932, 91, 94, 1, 0x00000000);
    //脸
    hdc_arc(4300 + base_x - defaultBasex, -1595 + base_y - defaultBasey, 4676, 243, 244, 1, 0x00000000);
    hdc_arc(-10 + base_x - defaultBasex, 635 + base_y - defaultBasey, 177, 64, 84, 1, 0x00000000);
    hdc_arc(267 + base_x - defaultBasex, 602 + base_y - defaultBasey, 102, 210, 263, 1, 0x00000000);
    hdc_arc(469 + base_x - defaultBasex, 341 + base_y - defaultBasey, 431, 194, 216, 1, 0x00000000);
    hdc_arc(380 + base_x - defaultBasex, 600 + base_y - defaultBasey, 161, 156, 186, 1, 0x00000000);
    hdc_arc(193 + base_x - defaultBasex, 241 + base_y - defaultBasey, 565, 148, 154, 1, 0x00000000);
    hdc_arc(219 + base_x - defaultBasex, 518 + base_y - defaultBasey, 111, 209, 287, 1, 0x00000000);
    hdc_arc(86 + base_x - defaultBasex, 586 + base_y - defaultBasey, 72, 31, 50, 1, 0x00000000);
    hdc_arc(203 + base_x - defaultBasex, 411 + base_y - defaultBasey, 110, 320, 17, 1, 0x00000000);
    hdc_arc(203 + base_x - defaultBasex, 349 + base_y - defaultBasey, 53, 324, 34, 1, 0x00000000);
    hdc_arc(487 + base_x - defaultBasex, 426 + base_y - defaultBasey, 151, 329, 30, 1, 0x00000000);
    hdc_arc(463 + base_x - defaultBasex, 339 + base_y - defaultBasey, 68, 323, 22, 1, 0x00000000);
    hdc_arc(245 + base_x - defaultBasex, 648 + base_y - defaultBasey, 152, 338, 11, 1, 0x00000000);
    hdc_arc(256 + base_x - defaultBasex, 527 + base_y - defaultBasey, 36, 333, 14, 1, 0x00000000);
    hdc_arc(270 + base_x - defaultBasex, 495 + base_y - defaultBasey, 6, 132, 300, 1, 0x00000000);
    hdc_arc(451 + base_x - defaultBasex, 483 + base_y - defaultBasey, 10, 253, 37, 1, 0x00000000);
    hdc_arc(477 + base_x - defaultBasex, 438 + base_y - defaultBasey, 37, 180, 199, 1, 0x00000000);
    hdc_arc(499 + base_x - defaultBasex, 627 + base_y - defaultBasey, 152, 338, 15, 1, 0x00000000);
    hdc_arc(245 + base_x - defaultBasex, 635 + base_y - defaultBasey, 124, 323, 16, 1, 0x00000000);
    hdc_arc(240 + base_x - defaultBasex, 594 + base_y - defaultBasey, 91, 311, 19, 1, 0x00000000);
    hdc_arc(284 + base_x - defaultBasex, 504 + base_y - defaultBasey, 14, 218, 253, 1, 0x00000000);
    hdc_arc(511 + base_x - defaultBasex, 676 + base_y - defaultBasey, 184, 338, 16, 1, 0x00000000);
    hdc_arc(506 + base_x - defaultBasex, 653 + base_y - defaultBasey, 171, 338, 20, 1, 0x00000000);
    hdc_arc(505 + base_x - defaultBasex, 553 + base_y - defaultBasey, 81, 307, 318, 1, 0x00000000);
    hdc_arc(338 + base_x - defaultBasex, 654 + base_y - defaultBasey, 23, 239, 343, 1, 0x00000000);
    hdc_arc(378 + base_x - defaultBasex, 715 + base_y - defaultBasey, 95, 331, 30, 1, 0x00000000);
    hdc_arc(411 + base_x - defaultBasex, 659 + base_y - defaultBasey, 31, 33, 73, 1, 0x00000000);
    hdc_arc(394 + base_x - defaultBasex, 646 + base_y - defaultBasey, 46, 95, 168, 1, 0x00000000);
    hdc_arc(381 + base_x - defaultBasex, 613 + base_y - defaultBasey, 81, 164, 231, 1, 0x00000000);
    hdc_arc(581 + base_x - defaultBasex, 769 + base_y - defaultBasey, 191, 261, 273, 1, 0x00000000);
    //清理一下外围
    hdc_square(800 + base_x - defaultBasex, 0 + base_y - defaultBasey, 800, 0, 1, 1, 0);
    hdc_square(-800 + base_x - defaultBasex, 0 + base_y - defaultBasey, 800, 0, 1, 1, 0);
    hdc_square(0 + base_x - defaultBasex, -800 + base_y - defaultBasey, 800, 0, 1, 1, 0);
    hdc_square(0 + base_x - defaultBasex, 800 + base_y - defaultBasey, 800, 0, 1, 1, 0);
}

