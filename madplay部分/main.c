#include "lcd.h"
#include "bmp.h"
#include "touch.h"
#include "filelist.h"
#include "play.h"
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

unsigned char if_next_pre = 1;
unsigned char *pic_ui = "/home/project/mp4player/pic/UI.bmp";
unsigned char *pic_mp3 = "/home/project/mp4player/pic/mp3.bmp";
void send_()
{
    Touch touch_data;
    char option;
    int x,y;
    int fd = open("/home/project/mp4player/test.fifo",O_WRONLY);
    int fd2 = open("/home/project/mp4player/stop.fifo",O_WRONLY);
    if(fd2 == -1)
    {
        perror("open stop error");
    }
    while(1)
    {
        get_touch_data(&touch_data);
        x = touch_data.x1;
        y = touch_data.y1;
        if(x > 380 && x < 434 && y > 430 && y < 480)//停止或继续
        {
            system("echo pause > /home/project/mp4player/test.fifo");
        }
        else if(x > 458 && x < 488 && y > 430 && y < 480)//下一个
        {   
            if_next_pre = 1;
            write(fd2,&if_next_pre,1);
            system("killall -9 mplayer");
        }
        else if(x > 323 && x < 351 && y > 430 && y < 480)//上一个
        {   
            if_next_pre = 0;
            write(fd2,&if_next_pre,1);
            system("killall -9 mplayer");
        }
        else if(x > 200 && x < 600 && y > 0 && y < 160)
        {
            if_next_pre = 3;
            write(fd2,&if_next_pre,1);
            system("killall -9 mplayer");
        }
        else if(x > 144 && x < 207 && y > 430 && y < 480)
        {
            system("echo seek -3 > /home/project/mp4player/test.fifo");
        }//回退
        else if(x > 227 && x < 286 && y > 430 && y < 480)
        {
            system("echo seek 3 > /home/project/mp4player/test.fifo");
        }//快放
        else if(x > 565 && x < 596 && y > 430 && y < 480)
        {
            system("echo volume 10 > /home/project/mp4player/test.fifo");
        }//音量+
        else if(x > 620 && x < 645 && y > 430 && y < 480)
        {
            system("echo volume -10 > /home/project/mp4player/test.fifo");
        }//音量-
    }
}





void play_(Pic *play_now)
{
    char filename[512];
    int fd2 = open("/home/project/mp4player/stop.fifo",O_RDONLY | O_NONBLOCK);
    while(1)
    {
        show_bmp(0,430,pic_ui);
        char file_ = *(play_now->pic_path+strlen(play_now->pic_path)-1);
        sprintf(filename,"mplayer -speed 1 -slave -zoom -x 800 -y 430 -input file=/home/project/mp4player/test.fifo %s",play_now->pic_path);
        printf("%s\n",filename);
        if(file_ == '4' || file_ == 'i')
        {
            system(filename);
        }
        else if(file_ == '3')
        {
            show_bmp(0,0,pic_mp3);
            system(filename);
        }
        
        read(fd2,&if_next_pre,1);
        if(if_next_pre == 3)
        {
            exit(0);
        }
        else if(if_next_pre == 0)
        {
            if_next_pre = 1;
            play_now = play_now->pre;
        }
        else if(if_next_pre == 1)
        {
            play_now = play_now->next;
        }
    }
}



int main()
{
    //初始化屏幕，触摸外屏，蜂鸣器。
    init_lcd();
    touch_init();
    int ret = mkfifo("/home/project/mp4player/test.fifo",0777);
    if(ret < -1)
    {
        perror("mkfifo error");
    }
    int ret2 = mkfifo("/home/project/mp4player/stop.fifo",0777);
    if(ret < -1)
    {
        perror("mkfifo error");
    }
    Head *p = creat_pic();
    get_bmp_pic(p);
    Pic *play_now = p->first;
    int wstatus;
    pid_t pid = fork();
    if(pid > 0)//父负责命令
    {
        send_();
    }
    else if(pid == 0)//子负责播放
    {
        play_(play_now);
    }
    return 0;
}
