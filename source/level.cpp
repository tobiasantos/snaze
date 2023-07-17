#include "../include/level.h"

#include <iostream>

void Level::set_cols(const int cols) {
    n_cols = cols;
}
void Level::set_rows(const int rows) {
    n_rows = rows;
}

void Level::set_spawn(const TilePos spawn) {
    snake_spawn_loc = spawn;
}

void Level::add_line(std::string line) {
    board.push_back(line);
}

void Level::set_tile_type(const TilePos pos, const tile_type_e type) {
    board[pos.row][pos.col] = tile_type[type];
}

size_t Level::get_cols() {
    return n_cols;
}

size_t Level::get_rows() {
    return n_rows;
}

TilePos Level::get_spawn() {
    return snake_spawn_loc;
}
TilePos Level::get_pellet_loc() {
    return pellet_loc;
}
        

tile_type_e Level::get_tile_type(const TilePos pos) {
    size_t aux = tile_type.find(board[pos.row][pos.col]);
    return static_cast<tile_type_e>(aux);
}

std::vector<TilePos> Level::empty_spaces() {
    std::vector<TilePos> aux;
     for(size_t i{0}; i < n_rows; ++i){
        for(size_t j{0}; j < n_cols; ++j) {
            if(board[i][j] == ' ') aux.push_back({i, j});
        }
    }

    return aux;
}


void Level::place_pellet() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<TilePos> es = empty_spaces();

    std::uniform_int_distribution<> random_index(0, es.size() - 1);

    pellet_loc = es[random_index(gen)];

    while(pellet_loc == snake_spawn_loc) 
        pellet_loc = es[random_index(gen)];

    board[pellet_loc.row][pellet_loc.col] = '@';
}

bool Level::crashed(const TilePos pos) {
    return (board[pos.row][pos.col] == '.' || board[pos.row][pos.col] == '#' || board[pos.row][pos.col] == 'o');
}

bool Level::is_food(const TilePos pos) {
    if (board[pos.row][pos.col] == '@') 
        return true;
    
    return false;
}


