#ifndef __MAP_H__
#define __MAP_H__
#include <iostream>
#include "lcd.h"
#include "touch.h"
#define CELL_SIZE 25
#define MAP_COLS 21
#define MAP_ROWS 16
using namespace std;

class Map
{
private:
    int bg_color;
    int map_color;
    screen *scr;
    rectangle *cell;
public:
    Map(screen *_scr,int _color = 0xffffff,int _map_color = 0x000000);

    ~Map();

    int get_map_color(){return map_color;};

    rectangle* get_cell(){return cell;};

    //void get_food(int color,int *if_have_food,int *if_game_over,Snake *snake);

    void Map_set();
};
#endif