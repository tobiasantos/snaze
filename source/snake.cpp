#include "../include/snake.h"

void Snake::set_lives(const unsigned short int l)
{
    lives = l;
}

unsigned short int Snake::get_lives()
{
    return lives;
}

MoveDir Snake::get_dir()
{
    return dir;
}

void Snake::set_dir(MoveDir d)
{
    dir = d;
}

std::deque<TilePos> Snake::get_body()
{
    return body;
}

void Snake::bind_level(Level *l)
{
    running_level = l;
}

void Snake::init()
{
    body.clear();
    body.push_back(running_level->get_spawn());
    running_level->set_tile_type(body.front(), tile_type_e::SNAKEHEAD);
    dir.dx = 0;
    dir.dy = 0;
}

TilePos Snake::get_next_location(MoveDir d)
{
    TilePos s_head = body.front();
    TilePos s_next;

    s_next.row = s_head.row + d.dy;
    s_next.col = s_head.col + d.dx;

    return s_next;
}

void Snake::step_foward(MoveDir d)
{

    TilePos s_next = get_next_location(d);

    if (!body.empty())
        running_level->set_tile_type(body.front(), tile_type_e::SNAKEBODY);

    body.push_front(s_next);

    if (!running_level->is_food(s_next))
    {
        running_level->set_tile_type(body.back(), tile_type_e::EMPTY);
        body.pop_back();
    }

    running_level->set_tile_type(s_next, tile_type_e::SNAKEHEAD);
}