/*!
 * @brief This file contains the implementation of a Player class.
 *
 * The Player class is responsible for manage the snake's movements. It has a
 * pointer to the snake and to the level it is currently in. There are two possible
 * ways to move the snake: randomly or using a backtracking algorithm.
 *
 * @author Tobias dos Santos Neto e Wisla Alves Argolo
 * @date July 16st, 2023.
 * @file player.h
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "snake.h"
#include "level.h"

/**
 * @class Player
 * @brief Class that represents a player.
 */
class Player
{
private:
  Snake *snake;                             //<! Pointer to the snake.
  Level *running_level;                     //<! Pointer to the level the player is currently in.
  std::vector<MoveDir> solution;            //<! Vector of MoveDir that represents the solution to the level.
  std::vector<MoveDir>::iterator curr_move; //<! Iterator to the current move.

  /**
   * @brief Get a random number between min and max.
   * @param min The minimum value.
   * @param max The maximum value.
   * @return A random number between min and max.
   */
  int random_chance(int min, int max);

  /**
   * @brief Check if two directions are not opposite.
   * @param dir One move direction.
   * @param curr The other move direction.
   * @return False if they are opposite, true otherwise.
   */
  bool is_valid(MoveDir dir, MoveDir curr);

public:
  /**
   * @brief Get the solution to the level.
   * @return A vector of MoveDir.
   */
  std::vector<MoveDir> get_solution();

  /**
   * @brief Find a solution to the level using the backtracking strategy with breadth first search.
   */
  void find_solution();

  /**
   * @brief Get the next move using the backtracking strategy.
   * @return The next move direction.
   */
  MoveDir next_move();

  /**
   * @brief Get the next move randomly.
   * @return The next move direction.
   */
  MoveDir next_move_random();

  /**
   * @brief Bind a level to the player.
   * @param l Pointer to the level.
   */
  void bind_level(Level *l);

  /**
   * @brief Bind a snake to the player.
   * @param s Pointer to the snake.
   */
  void bind_snake(Snake *s);
};

#endif