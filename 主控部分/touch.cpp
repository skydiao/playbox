#include "touch.h"
#include <iostream>
#include <string>
#include <linux/input.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
using namespace std;

struct input_event touch;//touch结构体保存存储的信息

void Point::set_x(int _x)
{
    x0 = _x;
}

void Point::set_y(int _y)
{
    y0 = _y;
}

int Point::get_x()
{
    return x0;
}

int Point::get_y()
{
    return y0;
}

TouchEvent::TouchEvent(string _name):devname(_name)
{
    cout << "touchevent 构造函数启动!" << endl;
    fd = open(devname.c_str(),O_RDONLY);
    if(fd == -1)
    {
        perror("open touch error");
    }
}   

TouchEvent::~TouchEvent()
{
    cout << "touchevent 析构函数启动!" << endl;
    close(fd);
}

void TouchEvent::get_touch_point(Point & point1,Point & point2)
{
    int x = -1;
    int y = -1;
    while(1)
    {
        //读取事件
        read(fd,&touch,sizeof(struct input_event));
        if(touch.type == EV_ABS)//触摸事件，转换坐标
        {
            if(touch.code == ABS_X)
            {
                x = touch.value*799.0/1023.0;
            }
            if(touch.code == ABS_Y)
            {
                y = touch.value*479.0/599.0;
            }
        }
        if(touch.type == EV_KEY&&touch.code == BTN_TOUCH)
        {//按键事件，保存按下，松开时的坐标
            if(touch.value == 1)
            {
                //touch_data->x0 = x;
                point1.set_x(x);
                cout << "x0 = " << x << endl;
                //touch_data->y0 = y;
                point1.set_y(y);
                cout << "y0 = " << y << endl;
            }
            else
            {
                //touch_data->x1 = x;
                point2.set_x(x);
                cout << "x0 = " << x << endl;
                //touch_data->y1 = y;
                point2.set_y(y);
                cout << "y0 = " << y << endl;
                return; 
            }
        }
    }
}

Direction TouchEvent::get_touch_direction(Point & point1,Point & point2)
{
    get_touch_point(point1,point2);
    //计算k值，也就是x，y的变化率
    int k = abs((point2.get_x()-point1.get_x())*1.0/(point2.get_y()-point1.get_y())*1.0);
    int x = point2.get_x()-point1.get_x();
    int y = point2.get_y()-point1.get_y();
    //以下分别表示上下左右
    if(k>1 && x>0)
    {
        return Direction::RIGHT;
    }
    else if(k>1 && x<0)
    {
        return Direction::LEFT;
    }
    else if(k<1 && y>0)
    {
        return Direction::DOWN;
    }
    else if(k<1 && y<0)
    {
        return Direction::UP;
    }
}


