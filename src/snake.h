#ifndef __SNAKE_H__
#define __SNAKE_H__
#include "winsys.h"

#include <chrono>
#include <vector>

class CSnake:public CFramedWindow
{
public:
  CSnake(CRect r, char _c = ' ');
  void paint();
  bool handleEvent(int c);

private:
    std::chrono::time_point<std::chrono::system_clock> time;
    unsigned int level;
    unsigned int speed;
    unsigned int body_len;
    bool display_help;
    bool is_paused;
    bool game_over;
    CPoint food;
    CPoint snake_head;
    std::vector<CPoint>snake_body;
    int snake_direction;
    void paint_help();
    void paint_food();
    void paint_level();
    void paint_snake();
    void paint_game_over();
    void generate_food();
    bool check_collision();
    bool move_snake(int c);
    void reset_game();
    void move_snake_border();
    void shift_snake();
    void move_window(int c);
    void eat_food();
    void paint_pause();
};

#endif
