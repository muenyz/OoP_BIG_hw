/* 2253731 �ƿ� ����� */
#include <iostream>
#include <Windows.h>
#include <fstream>
#include "bmp_subtitles.h"

using namespace std;

int myCeilDiv(int a, int b)
{
    return a / b + (a % b ? 1 : 0);
}

void readBmp(BITMAPFILEHEADER& fileHeader, BITMAPINFOHEADER& infoHeader,char*& data, char*& palette, const char* filename)
{
    data = palette = NULL;
    ifstream fin(filename, ios::binary);
    if (!fin)
    {
        cout << "�ļ���ʧ�ܣ�" << endl;
        exit(0);
    }
    fin.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER)); //��ȡ�ļ�ͷ
    if (fileHeader.bfType != 0x4D42) //�ж��Ƿ�Ϊbmp�ļ�
    {
        cout << "�ļ�����BMP��ʽ��" << endl;
        exit(0);
    }
    fin.read((char*)&infoHeader, sizeof(BITMAPINFOHEADER)); //��ȡ��Ϣͷ
    int bitCount = infoHeader.biBitCount;
    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;
    if (bitCount < 24)
    {
        palette = new char[(1 <<bitCount ) * 4];
        fin.read(palette, (1 << bitCount) * 4);
    }
    fin.seekg(fileHeader.bfOffBits, ios::beg);
    data=new(nothrow) char[fileHeader.bfSize - fileHeader.bfOffBits];
    if(!data)
    {
        cout<<"�ڴ����ʧ�ܣ�"<<endl;
        exit(0);
    }
    fin.read(data, fileHeader.bfSize - fileHeader.bfOffBits);
    fin.close();
}

bitmap_image::bitmap_image(const char* filename)
{
    readBmp(fileHeader, infoHeader, _data, _palette, filename);
}

int bitmap_image::width() const
{
    return infoHeader.biWidth;
}

int bitmap_image::height() const
{
    return infoHeader.biHeight;
}

int bitmap_image::bitCount() const
{
    return infoHeader.biBitCount;
}

void bitmap_image::coordToOffset(int row, int col, int& byteOff, int& bitOff) const
{
    int _bitCount = infoHeader.biBitCount;
    int _width = infoHeader.biWidth;
    int _height = infoHeader.biHeight;
    if (_bitCount == 24)
        byteOff = (_height - 1 - row) * (_width * 3 + _width % 4) + col * 3;//�ڶ��ӣ���Ҫ�ֽڶ���
    else if (_bitCount == 8)
        byteOff = (_height - 1 - row) * (_width + (4 - _width % 4) % 4) + col;
    else if (_bitCount == 4) {
        byteOff = ((_height - 1 - row) * (_width + (8 - _width % 8) % 8) + col) / 2;
        bitOff = ((_height - 1 - row) * (_width + (8 - _width % 8) % 8) + col) % 2;
    }
    else if (_bitCount == 1) {
        byteOff = ((_height - 1 - row) * (_width + (32 - _width % 32) % 32) + col) / 8;
        bitOff = ((_height - 1 - row) * (_width + (32 - _width % 32) % 32) + col) % 8;
    }
    else
        ;
    return;
}

unsigned int bitmap_image::get_pixel(int row, int col) const
{
    int pos = 0, bytePos = 0;
    coordToOffset(row, col, pos, bytePos);
    unsigned int color = 0;
    int _bitCount = infoHeader.biBitCount;
    if (_bitCount == 24)
    {
        color |= (unsigned int)(unsigned char)_data[pos++] << 16;
        color |= (unsigned int)(unsigned char)_data[pos++] << 8;
        color |= (unsigned int)(unsigned char)_data[pos++];//��ӣ�ûע���������1�����˰���ͼ��ô����
    }
    else if (_bitCount < 24) {
        unsigned int index = (_data[pos] >> (8 - _bitCount * (bytePos + 1))) & ((1 << _bitCount) - 1);
        color |= (unsigned int)(unsigned char)_palette[index * 4] << 16;
        color |= (unsigned int)(unsigned char)_palette[index * 4 + 1] << 8;
        color |= (unsigned int)(unsigned char)_palette[index * 4 + 2];
        void* p = _data + pos;
    }
    else
        ;
    return color;
}

//��bitΪ24ʱ��color��ʾrgbֵ��������24ʱ��color��ʾ��ɫ������
void bitmap_image::set_pixel(int row, int col, unsigned int color)
{
    int pos = 0, bytePos = 0;
    coordToOffset(row, col, pos, bytePos);
    int _bitCount = infoHeader.biBitCount;
    if (_bitCount == 24)
    {
        _data[pos++] = char(color >> 16);
        _data[pos++] = char(color >> 8);
        _data[pos++] = char(color);
    }
    else if (_bitCount < 24) {//Ҫ��data��posλ��bitCount*bytePos��ʼ��bitCountλ����color
        _data[pos] &= ~(((1 << _bitCount) - 1) << (8 - _bitCount * (bytePos + 1)));
        _data[pos] |= (color << (8 - _bitCount * (bytePos + 1)));
    }
    else
        ;
}

unsigned int bitmap_image::paletteIndex(unsigned int color)
{
    unsigned int index = 0;
    for (; index < ((unsigned)1 << infoHeader.biBitCount); index++)
        if (color == (((unsigned int)(unsigned char)_palette[index * 4] << 16) | ((unsigned int)(unsigned char)_palette[index * 4 + 1] << 8) | ((unsigned int)(unsigned char)_palette[index * 4 + 2])))
            break;
    return index;
}

void bitmap_image::save(const char* filename)
{
    ofstream fout(filename, ios::binary);
    fout.write((char*)&fileHeader, sizeof(BITMAPFILEHEADER));
    fout.write((char*)&infoHeader, sizeof(BITMAPINFOHEADER));
    if (infoHeader.biBitCount < 24)
        fout.write(_palette, (1 << infoHeader.biBitCount) * 4);
    fout.write(_data, fileHeader.bfSize - fileHeader.bfOffBits);
    fout.close();
}

void gb2312Trans(unsigned char& high, unsigned char& low)
{
    high -= 0xA0;
    low -= 0xA0;
}