/* 2253731 �ƿ� ����� */

#pragma once

#include <fstream>
#include <Windows.h>

using namespace std;
/* ������Ҫ�����Լ��Ķ��� */
void readBmp(BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& infoHeader, char*& data, char*& palette, const char* filename);
void gb2312Trans(unsigned char& high, unsigned char& low);
class bitmap_image
{
protected:
    /* ������Ҫ�����Լ��Ķ��� */
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    char* _data;	//ͼƬ������
    char* _palette;	//��ɫ��
    void coordToOffset(int row, int col, int& byteOff, int& bitOff) const;
public:
    int width() const;	//����ͼƬ�Ŀ��
    int height() const; //����ͼƬ�ĸ߶�
    unsigned int get_pixel(int row, int col) const; //����ָ�����RGB��ɫ

    /* ������Ҫ�����Լ��Ķ��� */
    int bitCount() const; //����ͼƬ��λ��
    bitmap_image(const char* filename); //���캯������ȡָ��bmp�ĸ�ʽ�����е����ɫ��Ϣ
    void set_pixel(int row, int col, unsigned int color);
    void save(const char* filename);
    unsigned int paletteIndex(unsigned int color);
    ~bitmap_image() //��������
    {
        if (_data)
            delete[] _data;
        if (_palette)
            delete[] _palette;
    }
};