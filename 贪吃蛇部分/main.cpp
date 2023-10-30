#include "lcd.h"
#include "touch.h"
#include "map.h"
#include "snake.h"
#include <thread>
#include <random>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

using namespace std;
screen *screen::only_screen = NULL;
//保存当前滑动的方向
Direction dir = Direction::LEFT;
//判断当前是否有食物存在
int food_flag = 0;
//用来结束线程
int if_game_over = 0;
//退出标志
int exit_flag = 0;

const char *pic_main = "./pic/main_page.bmp";
const char *pic_defeat = "./pic/defeat.bmp";
const char *pic_num[10] = {
        "./pic/0.bmp",
        "./pic/1.bmp",
        "./pic/2.bmp",
        "./pic/3.bmp",
        "./pic/4.bmp",
        "./pic/5.bmp",
        "./pic/6.bmp",
        "./pic/7.bmp",
        "./pic/8.bmp",
        "./pic/9.bmp"
};
Bmp defeat(pic_defeat);
Bmp _main(pic_main);

void change_dir(TouchEvent * Touch)
{
    Point p1;
    Point p2;
    while(!if_game_over)
    {
        dir = Touch->get_touch_direction(p1,p2);
    }
    cout << "t1 exit" << endl;
}

class Game
{
private:
    //初始化显示屏
    screen *scr;
    //初始化两点
    Point p1;
    Point p2;
    //初始化方向
    Direction position;
    //初始化触摸屏
    TouchEvent Touch;
    //地图初始化
    Map* map;
    Snake *snake;
    int score;
public:
    Game(const char *screen_path = "/dev/fb0"):scr(screen::set_screen(screen_path))
    {
        score = 0;
        cout << "game 初始化完成 " << endl;
    }

    int game_start()
    {
        _main.show_bmp(scr,0,0);
        while(1)
        {
            Touch.get_touch_point(p1,p2);
            if(p2.get_x() > 254 && p2.get_x() < 423 && p2.get_y() > 233 && p2.get_y() < 293)
            {
                exit_flag = 0;
                return 1;
            }
            else
            {
                //点击除了指定按键的任意区域三次退出游戏
                exit_flag++;
                if(exit_flag >= 3)
                {
                    exit(1);//正常退出
                }
            }
        }
    }


    int game_over()
    {
        //scr = screen::set_screen("/dev/fb0");
        cout << "game over page" << endl;
        defeat.show_bmp(scr,0,0);
        while(1)
        {
            Touch.get_touch_point(p1,p2);
            if(p2.get_x() > 413 && p2.get_x() < 536 && p2.get_y() > 252 && p2.get_y() < 308)
            {
                return 1;//再来一局
            }
            else if(p2.get_x() > 202 && p2.get_x() < 325 && p2.get_y() > 252 && p2.get_y() < 309)
            {
                return 0;//返回主界面
            }
        }
    }

    int game_run()
    {
        map = new Map(scr);
        snake = new Snake(map,scr);
        // 初始化线程
        thread t1(change_dir,&Touch);
        thread t2(&Snake::get_food,snake,0x00ff00,&food_flag,&if_game_over);

        int ret;
        while(1)
        {
            if(score <= 18)
            {
                usleep(8000*1000/(6+score));
            }
            else 
            {
                usleep(8000*1000/24);
            }
            ret = snake->move(dir,&food_flag,&if_game_over);
            if(!ret)
            {
                score = 0;

                Bmp defeat("./pic/defeat_.bmp");
                defeat.show_bmp(scr,0,0);
                t1.join();
                t2.join();
                return 3;
            }
            else if(ret == 2)
            {
                score++;
                int score_0 = score%10;
                int score_1 = score/10;
                Bmp num_0(pic_num[score_0]);
                Bmp num_1(pic_num[score_1]);
                num_0.show_bmp(scr,685,20);
                num_1.show_bmp(scr,650,20);
            }
        }
    }

    void game_reset()
    {
        dir = Direction::LEFT;
        //判断当前是否有食物存在
        food_flag = 0;
        //用来结束线程
        if_game_over = 0;
    }
};

int main()
{
    // screen *scr = screen::set_screen("/dev/fb0");
    // TouchEvent touch;
    // Map map(scr);
    // Snake snake(&map,scr);
    // // 初始化线程
    // thread t1(change_dir,&touch);
    // thread t2(&Snake::get_food,&snake,0x00ff00,&food_flag,&if_game_over);
    // while(1)
    // {
    //     sleep(1);
    //     int ret = snake.move(dir,&food_flag,&if_game_over);
    //     if(!ret)
    //     {
    //         break;
    //     }
    // }

    Game game;
    int ret = 0;//ret接受返回值判断进入哪一个界面
    while(1)
    {
        if(ret == 0)
        {
            ret = game.game_start();
        }
        if(ret == 1)
        {
            ret = game.game_run();
            game.game_reset();
        }
        if(ret == 2)
        {
            //排行榜
        }
        if(ret == 3)
        {
            //游戏结束
            ret = game.game_over();
        }
    }
}   