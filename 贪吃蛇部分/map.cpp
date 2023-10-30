#include <iostream>
#include <random>
#include <cmath>
#include "map.h"
using namespace std;

Map::Map(screen *_scr,int _color,int _map_color):bg_color(_color),map_color(_map_color),scr(_scr)
{
    int x = 0;
    int y = 0;
    cell = new rectangle(CELL_SIZE,CELL_SIZE,x,y,map_color);
    scr->flush_all(bg_color);
    for(int i = 0;i<MAP_ROWS;i++)
    {
        for(int j = 0;j<MAP_COLS;j++)
        {
            cell->set_x(x);
            cell->set_y(y);
            cell->draw_rectangle(scr);
            x+=(CELL_SIZE+5);
        }
        x = 0;
        y+=(CELL_SIZE+5);
    }
}

Map::~Map()
{
    delete cell;
}

// void Map::get_food(int color,int *if_have_food,int *if_game_over)
// {
//     // 以随机值播种，若可能
//     random_device r;
//     // 选择 1 与 15 间的随机数
//     std::default_random_engine e1(r());
//     std::uniform_int_distribution<int> uniform_dist(1, 15);
//     // 选择 1 与 20 间的随机数
//     std::uniform_int_distribution<int> uniform_cols(1, 20);
//     int rows = 0;
//     int cols = 0;
//     while(1)
//     {
//         if((*if_game_over) == 1)
//         {
//             cout << "thread get_food over" << endl;
//             break;
//         }
//         rows = uniform_dist(e1);
//         cols = uniform_cols(e1);
//         cell->set_x(cols*30);
//         cell->set_y(rows*30);
//         //没有食物并且不是蛇的身体
//         if((*if_have_food) == 0 && snake->if_belong_body(cols*30,rows*30))
//         {
//             cell->set_color(color);
//             cell->draw_rectangle(scr);
//             cell->set_food_if(1);
//             *if_have_food = 1;
//             cout << "food cols: " << cols << " " << "food rows: " << rows << endl;
//         }
//     }
// }
