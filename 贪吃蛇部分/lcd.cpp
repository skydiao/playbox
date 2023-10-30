#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <unistd.h>
#include "lcd.h"
using namespace std;

Shape::Shape(int _x,int _y,int _color):x0(_x),y0(_y),color(_color){}

Shape::~Shape()
{
    //cout << "shape 析构" << endl;
}

void Shape::set_color(int _color)
{
    color = _color;
}

void Shape::set_x(int _x)
{
    x0 = _x;
}

void Shape::set_y(int _y)
{
    y0 = _y;
}

int Shape::get_x() const
{
    return x0;
}

int Shape::get_y() const
{
    return y0;
}

int Shape::get_color() const
{
    return color;
}



screen::screen(const char *_path):path(_path)
{
    cout << "screen 构造函数启动!" << endl;
    fd = open(path,O_RDWR);
    if(-1 == fd)
    {
        perror("open fb0 error");
    }

    struct fb_var_screeninfo screen;
    ioctl(fd,FBIOGET_VSCREENINFO,&screen);
    width = screen.xres;
    high = screen.yres;
    bits_per_pixel = screen.bits_per_pixel;
    cout << "width: " << width << endl;
    cout << "high: " << high << endl;
    cout << "bits_per_pixel: " << bits_per_pixel << endl;
    
    plcd = (int *)mmap(NULL,width*high*bits_per_pixel/8,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(MAP_FAILED == plcd)
    {
        perror("mmap error");
    }
}

screen::~screen()
{
    cout << "screen 析构函数启动!" << endl;
    munmap(plcd,width*high*bits_per_pixel/8);
    close(fd);
}

screen *screen::set_screen(const char *_path)
{
    if(!only_screen)
    {
        only_screen = new screen(_path);
    }
    return only_screen;
}

void screen::draw_point(int x,int y,int color)
{
    if(x >= 0 && y >= 0 && x <= width && y <= high)
    {
        *(plcd + y*width + x) = color;
    }
}

void screen::flush_all(int color)
{
    for(int y = 0;y < high;y++)
    {
        for(int x = 0;x < width;x++)
        {
            draw_point(x,y,color);
        }
    }
}


void rectangle::draw_rectangle(screen *scr)
{
    for(int j = y0;j < y0 + high;j++)
    {
        for(int i = x0;i < x0 + width;i++)
        {
            scr->draw_point(i,j,color);
        }
    }
}

rectangle::rectangle(int _width ,int _high,int _x,int _y,int _color):Shape(_x,_y,_color),width(_width),high(_high),if_as_snake_body(0),if_food(0)
{
    //cout << "rectangle 构造函数启动" << endl;
}

rectangle::~rectangle()
{
    //cout << "rectangle 析构" << endl;
}

void rectangle::set_width(int _width)
{
    width = _width;
}

void rectangle::set_high(int _high)
{
    high = _high;
}



Bmp::Bmp(const char *path)
{
    cout << "bmp构造函数启动!" << endl;
    //cout << "构造函数内 bmp_path = " << bmp_path << endl;
    bmp_path = path;
    bmp_fd = open(bmp_path,O_RDONLY);
    if(bmp_fd < 0)
    {
        perror("open file error");
        return;
    }
    //读取打开的文件类型，不是bmp文件则退出
    read(bmp_fd,pic_buf,2);
    if(pic_buf[0] != 0x42 || pic_buf[1] != 0x4d)
    {
        cout << "这不是一个bmp文件!" << endl;
        return;
    }
    //读取光标位置
    lseek(bmp_fd,0x0A,SEEK_SET);
    read(bmp_fd,&offset,4);
    //读取图片宽度
    //以下有 | 符号的都是对小端模式的处理，低地址存低字节。
    lseek(bmp_fd,0x12,SEEK_SET);
    read(bmp_fd,pic_buf,4);
    bmp_width = pic_buf[3]<<24 | pic_buf[2]<<16 | pic_buf[1]<<8 | pic_buf[0];
    //读取图片长度
    read(bmp_fd,pic_buf,4);
    bmp_depth = pic_buf[3]<<24 | pic_buf[2]<<16 | pic_buf[1]<<8 | pic_buf[0];
    //读取图片色深
    lseek(bmp_fd,0x1c,SEEK_SET);
    read(bmp_fd,pic_buf,2);
    bmp_depth_color = pic_buf[1]<<8 | pic_buf[0];
    //算总像素点数目
    //先算每行的像素点数目
    fillbytes = 0;
    line_bytes = bmp_width*bmp_depth_color/8;
    if(line_bytes%4)
    {
        fillbytes = 4-line_bytes%4;
        line_bytes+=(4-line_bytes%4);
    }
    all_bytes = line_bytes*abs(bmp_depth);
    colorbuf = (unsigned char *)malloc(all_bytes);
    //读取每一个点的颜色
    //unsigned char colorbuf[all_bytes];
    lseek(bmp_fd,0x36,SEEK_SET);
    read(bmp_fd,colorbuf,all_bytes);
    //设置argb，根据小端模式来一个个画点
    //注意实际上在地址中保存的是b g r a，所以要移位处理
    cout << "pic_w=," << bmp_width;
    cout << "pic_h=," << bmp_depth;
    cout << "pic_depth=" << bmp_depth_color << endl;
    close(bmp_fd);
}

Bmp::~Bmp()
{
    cout << "bmp析构函数启动" << endl;
    free(colorbuf);
}

void Bmp::show_bmp(screen *scr,int x,int y)
{
    cout << "我进来了哦" << endl;
    cout << "bmp_path = " << bmp_path << endl;
    //设置argb，根据小端模式来一个个画点
    //注意实际上在地址中保存的是b g r a，所以要移位处理
    char a = 0,r,g,b;
    int count = 0,color;
    for(int i = 0;i<abs(bmp_depth);i++)
    {
        for(int j = 0;j<bmp_width;j++)
        {
            b = colorbuf[count++];
            g = colorbuf[count++];
            r = colorbuf[count++];
            if(bmp_depth_color == 32)
            {
                a = colorbuf[count++];
            }
            else
            {
                a = 0;
            }
            color = a <<24 | r << 16| g << 8 | b;
            if (bmp_depth > 0)
            {
                //bmp_depth > 0 时，从下往上画
                //lcd_draw_point(j + x,bmp_depth-1-i+y,color);
                scr->draw_point(j + x,bmp_depth-1-i+y,color);
            }
            else
            {
                //lcd_draw_point(j + x,i + y,color);
                scr->draw_point(j + x,i + y,color);
            }
        }
        count+=fillbytes;
    }
}