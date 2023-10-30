#ifndef __TOUCH_H__
#define __TOUCH_H__
#include <iostream>
#include <string>
using namespace std;
class Point
{
protected:
int x0;
int y0;

public:
Point(int _x = 0,int _y = 0):x0(_x),y0(_y){}

void set_x(int _x = 0);

void set_y(int _y = 0);

int get_x();

int get_y();
};

enum class Direction { ERROR, UP, DOWN, LEFT, RIGHT ,NO};

class TouchEvent
{
private:
    string devname;    //触摸屏设备文件名
    int fd;                 //文件描述符

public:
    //构造函数 
    TouchEvent(string _name="/dev/input/event0");

    //析构函数
    ~TouchEvent();

    //获取手指滑动的方向  
    Direction get_touch_direction(Point & point1,Point & point2);

    //获取手指点击的坐标 
    void get_touch_point(Point & point1,Point & point2);
};


#endif