/* 2253731 计科 张皓钦 */

#pragma once

#include <fstream>
#include <Windows.h>

using namespace std;
/* 根据需要加入自己的定义 */
void readBmp(BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& infoHeader, char*& data, char*& palette, const char* filename);
void gb2312Trans(unsigned char& high, unsigned char& low);
class bitmap_image
{
protected:
    /* 根据需要加入自己的定义 */
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    char* _data;	//图片的数据
    char* _palette;	//调色板
    void coordToOffset(int row, int col, int& byteOff, int& bitOff) const;
public:
    int width() const;	//返回图片的宽度
    int height() const; //返回图片的高度
    unsigned int get_pixel(int row, int col) const; //返回指定点的RGB颜色

    /* 根据需要加入自己的定义 */
    int bitCount() const; //返回图片的位数
    bitmap_image(const char* filename); //构造函数，读取指定bmp的格式及所有点的颜色信息
    void set_pixel(int row, int col, unsigned int color);
    void save(const char* filename);
    unsigned int paletteIndex(unsigned int color);
    ~bitmap_image() //析构函数
    {
        if (_data)
            delete[] _data;
        if (_palette)
            delete[] _palette;
    }
};