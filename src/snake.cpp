#include "snake.h"
#include "screen.h"
#include <curses.h>
#include <thread>

CSnake::CSnake(CRect r, char _c): CFramedWindow(r, _c) // wazneeeeeeeeee
{
    time = std::chrono::system_clock::now();
    body_len = 2;
    speed = 10;
    level = 1;
    display_help = true;
    is_paused = true;
    food.x = geom.topleft.x + 23;
    food.y = geom.topleft.y + 10;
    snake_head.y = geom.topleft.y + 2;
    snake_head.x = geom.topleft.x + 6;
    snake_direction = KEY_RIGHT;
    snake_body.push_back(CPoint(geom.topleft.x+5,geom.topleft.y+2));
    snake_body.push_back(CPoint(geom.topleft.x+4,geom.topleft.y+2));
    game_over = false;
}
bool CSnake::handleEvent(int c) {
    if(is_paused || game_over){
        if(CFramedWindow::handleEvent(c)){
            move_window(c);
            return true;
        }
    }
    eat_food();
    if(c == 'h'){
        display_help = !display_help;
        is_paused = display_help;
        return true;
    }
    if(c == 'p'){
        if(!display_help){
            is_paused = !is_paused;
            return true;
        }
    }
    if(c == 'r'){
        reset_game();
        return true;
    }
    time += std::chrono::milliseconds(1000/speed);
    this_thread::sleep_until(time);

    if(move_snake(c)){
        if(check_collision())
            game_over = true;
        return true;
    }
    if(!is_paused){
        move_snake(snake_direction);
        if(check_collision())
            game_over = true;
        return true;
    }
    return false;
}

void CSnake::paint() {
    CFramedWindow::paint();
    if(!game_over){
        paint_food();
        paint_level();
        paint_snake();
        if(display_help)
            paint_help();
        if(is_paused)
            paint_pause();
    }
    else{
        paint_game_over();
    }
}

void CSnake::paint_pause(){
    gotoyx( geom.topleft.y + geom.size.y+2, geom.topleft.x + (geom.size.x / 2) - 6);
    printl("Game paused!");
}

void CSnake::paint_help() {
    gotoyx(geom.topleft.y+4, geom.topleft.x+2);
    printl("h - toggle help information");
    gotoyx(geom.topleft.y+5, geom.topleft.x+2);
    printl("p - toggle pause/play mode");
    gotoyx(geom.topleft.y+6, geom.topleft.x+2);
    printl("r - restart game");
    gotoyx(geom.topleft.y+7, geom.topleft.x+2);
    printl("arrows - move snake (in play mode) or");
    gotoyx(geom.topleft.y+8, geom.topleft.x+2);
    printl("move window (in pause mode)");
}

void CSnake::paint_food() {
    gotoyx(food.y, food.x);
    printl("0");
}

void CSnake::paint_level(){
    gotoyx(geom.topleft.y-1, geom.topleft.x);
    printl("| Level: %d |", level);
}

void CSnake::paint_snake() {
    gotoyx(snake_head.y, snake_head.x);
    printl("*");
    for(auto &i : snake_body){
        gotoyx(i.y, i.x);
        printl("+");
    }
}

void CSnake::generate_food() {
    food.x = geom.topleft.x + 4 + rand() % (geom.size.x - 4);
    food.y = geom.topleft.y + 2 + rand() % (geom.size.y - 2);
    if(food.x == snake_head.x && food.y == snake_head.y){
        return generate_food();
    }
    for (auto &i : snake_body){
        if(food.x == i.x && food.y == i.y){
            return generate_food();
        }
    }
}

void CSnake::paint_game_over() {
    gotoyx(geom.topleft.y+1, geom.topleft.x+1);
    printl("Game over, result: %d", level);
}

bool CSnake::check_collision() {
    for(auto &i : snake_body){
        if(i.x == snake_head.x && i.y == snake_head.y)
            return true;
    }
    return false;
}

bool CSnake::move_snake(int c) {
    int x = 0, y = 0;
    switch (c) {
        case KEY_UP:
            if(snake_direction == KEY_DOWN){
                return false;
            }
            y -= 1;
            break;
        case KEY_DOWN:
            if(snake_direction == KEY_UP){
                return false;
            }
            y += 1;
            break;
        case KEY_LEFT:
            if(snake_direction == KEY_RIGHT){
                return false;
            }
            x -= 1;
            break;
        case KEY_RIGHT:
            if(snake_direction == KEY_LEFT){
                return false;
            }
            x += 1;
            break;
        default:
            return false;
    }
    shift_snake();
    snake_head.y += y;
    snake_head.x += x;
    move_snake_border();
    snake_direction = c;
    return true;
}

void CSnake::reset_game(){
    snake_body.clear();
    time = std::chrono::system_clock::now();
    body_len = 2;
    speed = 10;
    level = 1;
    display_help = true;
    is_paused = true;
    food.x = geom.topleft.x + 23;
    food.y = geom.topleft.y + 10;
    snake_head.y = geom.topleft.y + 2;
    snake_head.x = geom.topleft.x + 6;
    snake_direction = KEY_RIGHT;
    snake_body.push_back(CPoint(geom.topleft.x+5,geom.topleft.y+2));
    snake_body.push_back(CPoint(geom.topleft.x+4,geom.topleft.y+2));
    game_over = false;
}

void CSnake::move_snake_border() {
    if (snake_head.x == geom.topleft.x){
        snake_head.x = geom.topleft.x + geom.size.x - 2;
    }
    if (snake_head.y == geom.topleft.y){
        snake_head.y = geom.topleft.y + geom.size.y - 2;
    }
    if (snake_head.x == geom.topleft.x + geom.size.x - 1){
        snake_head.x = geom.topleft.x + 1;
    }
    if (snake_head.y == geom.topleft.y + geom.size.y - 1){
        snake_head.y = geom.topleft.y + 1;
    }
}

void CSnake::shift_snake() {
    if(!is_paused){
        if(body_len == snake_body.size()){
            for (unsigned int i = 0; i < snake_body.size() - 1; i++){
                snake_body[i] = snake_body[i + 1];
            }
        }
        snake_body.resize(body_len);
        snake_body[body_len - 1] = snake_head;
    }

}

void CSnake::move_window(int c) {
    int x = 0, y = 0;
    switch(c){
        case KEY_UP:
            y -= 1;
            break;
        case KEY_DOWN:
            y += 1;
            break;
        case KEY_RIGHT:
            x += 1;
            break;
        case KEY_LEFT:
            x -= 1;
            break;
        default:
            break;
    }
    snake_head.x += x;
    snake_head.y += y;
    for(auto &i : snake_body){
        i.x += x;
        i.y += y;
    }
    food.x += x;
    food.y += y;
}

void CSnake::eat_food() {
    if(snake_head.y == food.y && snake_head.x == food.x){
        body_len++;
        level++;
        speed++;
        generate_food();
    }
}