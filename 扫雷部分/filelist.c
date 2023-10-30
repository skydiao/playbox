#include "lcd.h"
#include "touch.h"
#include "filelist.h"
#include "bmp.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
char *main_pic_path = "./main_page.bmp";
Touch touch_data;
int exit_flag = 0;
//页面接口，也就是第一个界面
void page_change()
{
    show_bmp(0,0,main_pic_path);
    int x = 0,y = 0;
    while(1)
    {
        show_bmp(0,0,main_pic_path);
        get_touch_data(&touch_data);
        x = touch_data.x1;
        y = touch_data.y1;
        if(x > 254 && x < 422 && y > 230 && y < 294)
        {
            exit_flag = 0;
            //进入主游戏界面
            game_run();
            return;
        }
        else if(x > 254 && x < 422 && y > 325 && y < 382)
        {
            exit_flag = 0;
            //进入排行榜界面
            show_rank();
        }
        else
        {
            exit_flag++;
            if(exit_flag == 3)
            {
                exit(1);//正常退出游戏
            }
        }
    }
}