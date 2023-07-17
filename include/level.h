/*!
 * @brief This file contains the implementation of a Level class.
 *
 * The Level class is responsible for storing the game level, which is
 * represented by a vector of strings. Each character represent the
 * following:
 *
 * - '#' : wall
 * - ' ' : empty space
 * - '@' : snake head
 * - '*' : snake body
 * - 'o' : food
 *
 * @author Tobias dos Santos Neto e Wisla Alves Argolo
 * @date July 16st, 2023.
 * @file level.h
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include <random>

/**
 * @brief Struct that represents a tile position.
 */
struct TilePos
{
    size_t row; //!< The row of the tile.
    size_t col; //!< The column of the tile.

    /**
     * @brief Check if two TilePos are equal.
     * @param other The other TilePos.
     * @return True if they are equal, false otherwise.
     */
    bool operator==(const TilePos &other) const
    {
        return row == other.row && col == other.col;
    }

    /**
     * @brief Check if two TilePos are different.
     * @param other The other TilePos.
     * @return True if they are different, false otherwise.
     */
    bool operator!=(const TilePos &other) const
    {
        return !(*this == other);
    }

    /**
     * @brief Copy assignment operator.
     * @param other The other TilePos.
     * @return A reference to the TilePos.
     */
    TilePos &operator=(const TilePos &other)
    {
        if (this != &other)
        {
            this->row = other.row;
            this->col = other.col;
        }
        return *this;
    }
};

/**
 * @brief Enum that represents the tile types.
 */
enum tile_type_e
{
    EMPTY = 0,
    WALL,
    INVISIBLE,
    FOOD,
    SNAKEHEAD,
    SNAKEBODY
};

/**
 * @class Level
 * @brief Class that represents a level of the game.
 */
class Level
{
private:
    std::vector<std::string> board; //!< The board of the level.
    TilePos snake_spawn_loc;        //!< The spawn location of the snake.
    TilePos pellet_loc;             //!< The location of the pellet.
    int n_cols;                     //!< The number of columns of the level.
    int n_rows;                     //!< The number of rows of the level.

    std::string tile_type = " #.@*o"; //!< The string that represents the tile types.

public:
    /**
     * @brief Sets the number of columns and rows of the level.
     * @param cols The number of columns of the level.
     */
    void set_cols(const int cols);

    /**
     * @brief Sets the number of rows of the level.
     * @param rows The number of rows of the level.
     */
    void set_rows(const int rows);

    /**
     * @brief Sets the spawn location of the snake.
     * @param spawn The spawn location of the snake.
     */
    void set_spawn(const TilePos spawn);

    /**
     * @brief Get the columns amount of the level.
     * @return The columns amount of the level.
     */
    size_t get_cols();

    /**
     * @brief Get the rows amount of the level.
     * @return The rows amount of the level.
     */
    size_t get_rows();

    /**
     * @brief Get the spawn location of the snake.
     * @return The spawn location of the snake.
     */
    TilePos get_spawn();

    /**
     * @brief Get the location of the pellet.
     * @return The location of the pellet.
     */
    TilePos get_pellet_loc();

    /**
     * @brief Add a line to the level.
     * @param line The line to be added.
     */
    void add_line(std::string line);

    /**
     * @brief Get the empty spaces of the level.
     * @return The empty spaces of the level.
     */
    std::vector<TilePos> empty_spaces();

    /**
     * @brief Set the tile type at the given position.
     * @param pos The position to be set.
     * @param type The type to be set.
     */
    void set_tile_type(const TilePos pos, const tile_type_e type);

    /**
     * @brief Get the tile type at the given position.
     * @param pos The position to be checked.
     * @return The tile type at the given position.
     */
    tile_type_e get_tile_type(const TilePos pos);

    /**
     * @brief Places a food pellet in a random empty space.
     */
    void place_pellet();

    /**
     * @brief Checks if the tile at the given position is a wall, an invisible wall or the snake body.
     * @param pos The position to be checked.
     */
    bool crashed(const TilePos pos);

    /**
     * @brief Checks if the tile at the given position is a food pellet.
     * @param pos The position to be checked.
     */
    bool is_food(const TilePos pos);
};

#endif