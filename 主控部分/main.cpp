#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "lcd.h"
#include "touch.h"

using namespace std;
const char *pic_start = "./pic/start.bmp"; 

int exit_flag = 0;
screen *screen::only_screen = nullptr;
int main()
{
    screen *scr = screen::set_screen("/dev/fb0");
    Point p1;
    Point p2;
    TouchEvent touch;
    Bmp start(pic_start);
    start.show_bmp(scr,0,0);
    while(1)
    {
        touch.get_touch_point(p1,p2);
        //扫雷
        if(p2.get_x() > 86 && p2.get_x() < 249 && p2.get_y() > 253 && p2.get_y() < 350)
        {
            exit_flag = 0;
            pid_t pid = fork();
            if(pid == 0) //子进程
            {
                chdir("/home/project/Minesweeper");
                system("./Minesweeper");
            } 
            else if(pid > 0) //父进程
            {
                waitpid(pid,NULL,0);
            }
            start.show_bmp(scr,0,0);
        }//贪吃蛇
        else if(p2.get_x() > 302 && p2.get_x() < 466 && p2.get_y() > 142 && p2.get_y() < 240)
        {
            exit_flag = 0;
            pid_t pid = fork();
            if(pid == 0) //子进程
            {
                chdir("/home/project/snake");
                system("./snake");
            } 
            else if(pid > 0) //父进程
            {
                waitpid(pid,NULL,0);
            }
            start.show_bmp(scr,0,0);
        }//播放器
        else if(p2.get_x() > 538 && p2.get_x() < 699 && p2.get_y() > 252 && p2.get_y() < 351)
        {
            exit_flag = 0;
            pid_t pid = fork();
            if(pid == 0) //子进程
            {
                chdir("/home/project/mp4player");
                system("./Mp4player");
            } 
            else if(pid > 0) //父进程
            {
                waitpid(pid,NULL,0);
            }
            start.show_bmp(scr,0,0);
        }
        else 
        {
            exit_flag++;
            if(exit_flag == 3)
            {
                exit(1);
            }
        }
    }
}