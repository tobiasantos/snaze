/*!
 * @brief This file contains the implementation of a Snake class.
 *
 *  The Snake class is responsible for storing the snake's body, its direction,
 *  its lives and the number of steps it has taken. It also has a pointer to the
 *  level it is currently in.
 *
 * @author Tobias dos Santos Neto e Wisla Alves Argolo
 * @date July 16st, 2023.
 * @file snake.h
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <deque>

#include "level.h"

/**
 * @brief Struct that represents a move direction.
 */
struct MoveDir
{
    short dx; //<! The x direction.
    short dy; //<! The y direction.

    /**
     * @brief Turn the snake left.
     */
    void turn_left()
    {
        short aux = dx;
        dx = dy;
        dy = -aux;
    }

    /**
     * @brief Turn the snake right.
     */
    void turn_right()
    {
        short aux = dy;
        dy = dx;
        dx = -aux;
    }

    /**
     * @brief Check if two MoveDir are equal.
     * @param other The other MoveDir.
     * @return True if they are equal, false otherwise.
     */
    bool operator==(const MoveDir &other) const
    {
        return dx == other.dx && dy == other.dy;
    }
};

/**
 * @class Snake
 * @brief Class that represents a snake.
 */
class Snake
{
private:
    std::deque<TilePos> body;     // <! Vector of TilePos that represents the snake's body.
    MoveDir dir;                  // <! Direction the snake is facing.
    unsigned short int lives = 5; // <! Number of lives the snake has.
    Level *running_level;         // <! Pointer to the level the snake is currently in.

public:
    /**
     * @brief Moves the snake one step foward.
     * @param d Direction the snake is facing.
     */
    void step_foward(MoveDir d);

    /**
     * @brief Get the snake's body.
     * @return Deque of TilePos that represents the snake's body.
     */
    std::deque<TilePos> get_body();

    /**
     * @brief Get the snake's direction.
     * @return Direction the snake is facing.
     */
    MoveDir get_dir();

    /**
     * @brief Get the snake's lives.
     * @return Number of lives.
     */
    unsigned short int get_lives();

    /**
     * @brief Sets the snake's direction.
     * @param d Direction the snake is facing.
     */
    void set_dir(const MoveDir d);

    /**
     * @brief Sets the snake's lives.
     * @param l Number of lives.
     */
    void set_lives(const unsigned short int l);

    /**
     * @brief Get the snake's next location in the direction d.
     * @param d Direction the snake is facing.
     * @return The snake's next location.
     */
    TilePos get_next_location(MoveDir d);

    /**
     * @brief Initializes the snake.
     */
    void init();

    /**
     * @brief Binds the snake to a level.
     * @param l Pointer to the level.
     */
    void bind_level(Level *l);
};

#endif