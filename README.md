90-02-b2项目中绘画卡通人物时可用helloworld.py来描绘轮廓。
helloworld.py具体功能为单击坐标参考点后，再连续单击3次可生成oop项目中相应的cpp语句用于绘制圆弧或者三角形并输出到a.txt文件中用于完成作业使用。当三点足够接近时，不断描绘圆弧可拟合原图的曲线，从而对参考的卡通人物完成描图。
输出的语句部分示例如下：（通过程序自动生成语句可以剩下特别多的时间）
```cpp
    //额头
    hdc_sector(508 + base_x - defaultBasex, 346 + base_y - defaultBasey, 369, 297, 330, 1, 1, 0x00ddf3ff);
    hdc_sector(287 + base_x - defaultBasex, 288 + base_y - defaultBasey, 265, 9, 58, 1, 1, 0x00ddf3ff);
    hdc_arc(906 + base_x - defaultBasex, 500 + base_y - defaultBasey, 785, 279, 294, 22, 0x00ddf3ff);
    hdc_arc(273 + base_x - defaultBasex, 299 + base_y - defaultBasey, 268, 58, 97, 30, 0x00ddf3ff);
    hdc_arc(74 + base_x - defaultBasex, 396 + base_y - defaultBasey, 488, 75, 92, 20, 0x00ddf3ff);
    hdc_arc(-13 + base_x - defaultBasex, 396 + base_y - defaultBasey, 585, 73, 98, 1, 0x00ddf3ff);
    hdc_circle(254 + base_x - defaultBasex, 271 + base_y - defaultBasey, 103, 1, 1, 0x00ddf3ff);
    hdc_circle(462 + base_x - defaultBasex, 246 + base_y - defaultBasey, 86, 1, 1, 0x00ddf3ff);
```
最终效果如下：

![QQ截图20231028175022](https://github.com/user-attachments/assets/4be21249-3223-48eb-811d-d2bb81fe87f9)

原图如下：

![CD9T08437F@B%G OV9N0T$E](https://github.com/user-attachments/assets/a0a8cc5c-725f-44d3-9013-608633ba5622)

