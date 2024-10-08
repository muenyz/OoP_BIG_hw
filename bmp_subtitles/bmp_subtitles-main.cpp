/*2253731 计科 张皓钦*/
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "../include/class_aat.h"
#include "bmp_subtitles.h"

using namespace std;

static void usage(const char* const fullpath_procname)
{
    const char* procname = strrchr(fullpath_procname, '\\')+1;
    if (procname == NULL)
        procname = fullpath_procname;

    cout << endl;
    cout << "Usage: " << procname << " [--bmpfile] [--outfile] [--traditional] [--posx] [--posy] [--color] [--style] [--subtitles]" << endl;
    cout << endl;
    cout << "需要的参数:" << endl;
    cout << "    --bmpfile\t\t给出原图文件名" << endl;
    cout << "    --outfile\t\t给出目标文件名" << endl;
    cout << "    --traditional\t设置为繁体字（可选）" << endl;
    cout << "    --posx\t\t第一个字的x坐标" << endl;
    cout << "    --posy\t\t第一个字的y坐标" << endl;
    cout << "    --color\t\t字幕的颜色（仅24bit下有效，默认为黑，可选）" << endl;
    cout << "    --style\t\thorizontal为水平，vertical为竖直（可选）" << endl;
    cout << "    --subtitles\t\t要添加的内容" << endl;
    cout << endl;
}

int main(int argc, char** argv)
{
    if (argc <6) {
        usage(argv[0]);
        return 0;
    }
    string style[] = { "horizontal","vertical",""};
    args_analyse_tools args[] = {
        args_analyse_tools("--bmpfile", ST_EXTARGS_TYPE::str,          1, string("")),
        args_analyse_tools("--outfile",    ST_EXTARGS_TYPE::str,          1, string("")),
        args_analyse_tools("--traditional",    ST_EXTARGS_TYPE::boolean, 0,false),
        args_analyse_tools("--posx",    ST_EXTARGS_TYPE::int_with_error, 1, 0, 0, 1919),
        args_analyse_tools("--posy",    ST_EXTARGS_TYPE::int_with_error, 1, 0, 0, 1079),
        args_analyse_tools("--color",    ST_EXTARGS_TYPE::str, 1, string("000000")),
        args_analyse_tools("--style",    ST_EXTARGS_TYPE::str_with_set_default,1,0,style),
        args_analyse_tools("--subtitles",    ST_EXTARGS_TYPE::str, 1, string("")),
        args_analyse_tools()  //最后一个，用于结束
    };
    args_analyse_process(argc, argv, args, 0);
    if (!args[0].existed() || !args[1].existed() || !args[3].existed() || !args[4].existed() || !args[7].existed()) {
        usage(argv[0]);
        return 0;
    }
    bitmap_image bmp(args[0].get_string().c_str());
    if (args[3].get_int() < 0 || args[4].get_int() < 0) {
        cout << "posx和posy应大于0！" << endl;
        return 0;
    }
    else {
        int lastx = args[3].get_int() + 16 * args[7].get_string().length()/2;
        int lasty = args[4].get_int() + 16;
        if (lastx > bmp.width() || lasty > bmp.height()) {
            cout << "字幕坐标非法！（某个字中某部分的坐标会超过图像的高或者宽）" << endl;
            return 0;
        }
    }
    ifstream hzk(args[2].existed() ? "hzk16f" : "hzk16", ios::binary);
    if(!hzk){
        cout<<"文件打开失败！"<<endl;
        return 0;
    }
    int posx = args[3].get_int();
    int posy = args[4].get_int();
    unsigned int color = stoi(args[5].get_string(), NULL, 16);
    if (bmp.bitCount() < 24) {
        color=bmp.paletteIndex(0x000000);
    }
    int Xstep = 0, Ystep = 0;
    if (args[6].get_string() == "horizontal")
        Xstep = 16;
    else
        Ystep = 16;


    string subtitles = args[7].get_string();
    int len = subtitles.length();
    unsigned char ch1, ch2;
    for (int i = 0; i < len;i++,posx+=Xstep,posy+=Ystep) {
        ch1 = subtitles[i++];
        ch2 = subtitles[i];
        gb2312Trans(ch1, ch2);
        unsigned int ch = (94*(ch1-1)+(ch2-1));
        char buffer[32];
        hzk.seekg(ch * 32, ios::beg);
        hzk.read(buffer, 32);
        for (int dy = 0; dy < 16; dy++)
            for (int dx = 0; dx < 2; dx++)
                for (int bit = 0; bit < 8; bit++)
                    if (buffer[dy * 2 + dx] & (1 << (7 - bit)))
                        bmp.set_pixel(posy + dy, posx + 8 * dx + bit, color);
    }
    bmp.save(args[1].get_string().c_str());
    hzk.close();
    return 0;
}
