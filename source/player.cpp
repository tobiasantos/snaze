#include <iostream>
#include <random>
#include <vector>
#include <stack>
#include <set>
#include <queue>

#include "../include/player.h"

void Player::bind_level(Level *l)
{
  running_level = l;
}

void Player::bind_snake(Snake *s)
{
  snake = s;
}

MoveDir Player::next_move_random() 
{
  MoveDir dir = snake->get_dir();
  std::vector<MoveDir> possible_dirs;

  if (dir == MoveDir{0,0}) {
    // if direction is neutral (game start), all directions are possible 
    possible_dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    dir = MoveDir{0,1};
  } else {
    // otherwise, possible directions are forward, left and right
    MoveDir dir_left = dir;
    dir_left.turn_left();
    possible_dirs.push_back(dir_left);

    possible_dirs.push_back(dir); 

    MoveDir dir_right = dir;
    dir_right.turn_right();
    possible_dirs.push_back(dir_right);
    
  }

  std::vector<MoveDir> valid_dirs;
  for (MoveDir d : possible_dirs) {
    TilePos pos = snake->get_next_location(d);
    if (!running_level->crashed(pos) || snake->get_body().back() == pos) 
        valid_dirs.push_back(d);
    
  }

  return valid_dirs.empty() ? dir : valid_dirs[random_chance(0, valid_dirs.size() - 1)];
}

int Player::random_chance(int min, int max)
{
  if (min == max)
    return min;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min, max);

  return dis(gen);
}

MoveDir Player::next_move() {
  std::vector<MoveDir>::iterator aux = curr_move;
  ++curr_move;
  return *aux;
}

bool Player::is_valid(MoveDir dir, MoveDir curr) {
  return !(dir.dx + curr.dx == 0 && dir.dy + curr.dy == 0);
}

std::vector<MoveDir> Player::get_solution() {
  return solution;
}

void Player::find_solution() {
  struct State {
    TilePos position;
    std::vector<MoveDir> directions;
    std::deque<TilePos> snake_state;
  };

  solution.clear();

  std::queue<State> search; 
  std::set<std::string> visited;

  std::vector<MoveDir> possible_dirs = {{0,1}, {0,-1}, {-1, 0}, {1,0}};

  State initial = {snake->get_body()[0], {{snake->get_dir()}}, snake->get_body()};
  search.push(initial);

  while(!search.empty()) {
    State curr = search.front(); 
    search.pop();

    std::string key = std::to_string(curr.position.row) + "," + std::to_string(curr.position.col)
                      + "," + std::to_string(curr.directions.back().dx) + "," + std::to_string(curr.directions.back().dy);

    if(visited.count(key) > 0) continue;

    visited.insert(key);

    solution = curr.directions;

    if(running_level->is_food(curr.position)) {
      solution.erase(solution.begin());
      curr_move = solution.begin();
      return;
    }


    for(auto d : possible_dirs) {
      if(!is_valid(d, curr.directions.back())) continue;

      TilePos pos = {curr.position.row + d.dy, curr.position.col + d.dx};

      bool body = false;
      for(auto i{0}; i < curr.snake_state.size(); ++i) {
        if(pos == curr.snake_state[i] && curr.snake_state.back() != pos) body = true;
      }

      if(running_level->get_tile_type(pos) == tile_type_e::WALL || 
      running_level->get_tile_type(pos) == tile_type_e::INVISIBLE || body) continue; 

      std::vector<MoveDir> new_dirs = curr.directions;
      new_dirs.push_back(d);

      std::deque<TilePos> new_snake = curr.snake_state;

      new_snake.push_front(pos);
      new_snake.pop_back();

      search.push({pos, new_dirs, new_snake}); 
    }
    
  }
  solution.clear();
}