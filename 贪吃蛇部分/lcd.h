#ifndef __LCD_H__
#define __LCD_H__

class screen
{
public:
    static screen *set_screen(const char *_path);
    void draw_point(int x,int y,int color);
    void flush_all(int color);

private:
    const char *path;
    int fd;
    int width;
    int high;
    int bits_per_pixel;
    int *plcd;
    static screen *only_screen;

private:
    screen(const char *_path);
    ~screen();
};

class Bmp
{
private:
    const char *bmp_path;//图片路径
    int bmp_fd;//图片的文件描述符
    unsigned char pic_buf[4];
    int offset;//保存光标位置
    int bmp_width,bmp_depth;//保存图片的宽和长
    int bmp_depth_color;//保存图片的色深
    int fillbytes;//要补充的像素点数目
    int line_bytes;//每行的像素点数目
    int all_bytes;//总像素点数目
    unsigned char * colorbuf;//保存每一个像素点的信息

public:
    Bmp(const char *path);

    ~Bmp();

    void show_bmp(screen * scr,int x,int y);
};

class Shape
{
protected:
    int x0;
    int y0;
    int color;

public:
    Shape(int _x = 0,int _y = 0,int _color = 0);

    ~Shape();

    void set_color(int _color);
    void set_x(int _x);
    void set_y(int _y);

    int get_x() const;
    int get_y() const;
    int get_color() const;
};

class rectangle:public Shape
{
protected:
    int width;
    int high;
    int if_as_snake_body;
    int if_food;

public:
    rectangle(int _width = 0,int _high = 0,int _x = 0,int _y = 0,int _color = 0);
    ~rectangle();
    void set_width(int _width);
    void set_high(int _high);
    void draw_rectangle(screen *scr);
    int get_if_snake_body(){return if_as_snake_body;};
    void set_if_snake_body(int _if_as_snake_body){if_as_snake_body = _if_as_snake_body;};
    int get_food_if(){return if_food;};
    void set_food_if(int _if_food){if_food = _if_food;};
};

#endif