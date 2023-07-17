/*!
 * @brief This file contains the implementation of a Simulation class.
 *
 * The Simulation class is responsible for managing the game. It has a vector of
 * levels, a snake and a player. It also has a iterator to the level the player is
 * currently in. The Simulation class is responsible for updating the game state,
 * rendering the game and processing the events.
 *
 * @author Tobias dos Santos Neto e Wisla Alves Argolo
 * @date July 16st, 2023.
 * @file simulation.h
 */

#ifndef GAMESNAKE_H
#define GAMESNAKE_H

#include "level.h"
#include "snake.h"
// #include "randomsplayer.h"
// #include "btsplayer.h"
#include "player.h"
#include <iostream>

namespace snz
{

    /**
     * @brief Struct that represents the running options.
     */
    struct RunningOptions
    {
        int n_food_pellets_per_nivel = 10;        //<! The number of food pellets per level.
        unsigned short int lives = 5;             //<! The number of lives.
        int fps = 12;                             //<! The frames per second.
        std::string player_type = "backtracking"; //<! The player type.
        std::string file_name;                    //<! The file name.
    };

    /**
     * @brief Enum that represents the simulation result.
     */
    enum simulation_result_e
    {
        OK,
        ERROR
    };

    /**
     * @brief Enum that represents the simulation state.
     */
    enum simulation_state_e
    {
        START,
        THINKING,
        RUN,
        CRASH,
        LEVEL_UP,
        GAME_OVER
    };

    /**
     * @brief Struct that represents the simulation result.
     */
    struct SimulationResult
    {
        simulation_result_e type; //<! The type of the result.
        std::string msg;          //<! The message of the result.
    };

    /**
     * @class SnakeSimulation
     * @brief Class that represents the simulation.
     */
    class SnakeSimulation
    {
    private:
        std::vector<Level> levels; //<! The levels.
        Snake snake;               //<! The snake.
        // SPlayer* player;
        Player player;                              //<! The player.
        std::vector<Level>::iterator running_level; //<! Iterator to the current level.
        simulation_state_e state;                   //<! The current state of the simulation.
        int consumed_pellets = 0;                   //<! The number of consumed pellets.
        float score = 0;                            //<! The score of the player.
        RunningOptions opt;                         //<! The running options.

        /**
         * @brief Read the levels from a file.
         */
        void read_levels();

        /**
         * @brief Initialize the running level.
         */
        void init_running_level();

        /**
         * @brief Render the board.
         */
        void render_board();

        /**
         * @brief Repeat a string n times.
         * @param s The string to be repeated.
         * @param n The number of times to repeat.
         * @return The repeated string.
         */
        std::string repeat_string(const std::string s, int n);

        /**
         * @brief Print the lost message.
         */
        void print_lost();

        /**
         * @brief Print the won message.
         */
        void print_won();

        /**
         * @brief Print the crashed message.
         */
        void print_crashed();

        /**
         * @brief Print the level up message.
         */
        void print_level_up();

        /**
         * @brief Print the welcome message.
         */
        void print_welcome();

        /**
         * @brief Print the game options.
         */
        std::string options();

    public:
        // ~SnakeSimulation() { delete player; }

        /**
         * @brief Update the game state.
         */
        void update();

        /**
         * @brief Initialize the game based on received arguments.
         * @param argc The number of arguments.
         * @param argv The arguments.
         * @return A SimulationResult with a message and it's type: ERROR or OK.
         */
        SimulationResult initialize(int argc, char *argv[]);

        /**
         * @brief Process the events of the game.
         */
        void process_events();

        /**
         * @brief Render the game based on the current state.
         */
        void render();

        /**
         * @brief Check if the game is over.
         * @return True if the game is over, false otherwise.
         */
        bool game_over();
    };

};

#endif