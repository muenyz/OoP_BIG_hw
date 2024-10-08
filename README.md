90-02-b2项目中绘画卡通人物时可用helloworld.py来描绘轮廓。
helloworld.py具体功能为单击坐标参考点后，再连续单击3次可生成oop项目中相应的cpp语句用于绘制圆弧或者三角形并输出到a.txt文件中用于完成作业使用。当三点足够接近时，不断描绘圆弧可拟合原图的曲线，从而对参考的卡通人物完成描图。
输出的语句部分示例如下：（通过程序自动生成语句可以剩下特别多的时间）
```cpp
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
```
最终效果如下：

![QQ截图20231028175022](https://github.com/user-attachments/assets/4be21249-3223-48eb-811d-d2bb81fe87f9)

原图如下：

![CD9T08437F@B%G OV9N0T$E](https://github.com/user-attachments/assets/a0a8cc5c-725f-44d3-9013-608633ba5622)

