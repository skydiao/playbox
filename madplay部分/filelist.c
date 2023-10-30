#include "lcd.h"
#include "touch.h"
#include "filelist.h"
#include "bmp.h"
#include <stdio.h>
char *main_pic_path = "./main_page.bmp";
Touch touch_data;
//页面接口，也就是第一个界面
void page_change()
{
    show_bmp(0,0,main_pic_path);
    int x = 0,y = 0;
    while(1)
    {
       
    }
}