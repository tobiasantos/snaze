#include "../include/simulation.h"

#include <iostream>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <fstream>
#include <chrono>
#include <thread>

#include <cstdlib>

std::string snz::SnakeSimulation::options()
{
    std::ostringstream oss;
    oss << "Usage: snaze [<options>] <input_level_file>\n"
        << "   Game simulation options:\n"
        << "       --help                  Print this help text.\n"
        << "       --fps <num>             Number of frames (board) presented per second. Default = 12. Valid range = [1, 50]\n"
        << "       --lives <num>           Number of lives the snake shall have. Default = 5. Valid range = [1, 20]\n"
        << "       --food <num>            Number of food pellets for the entire simulation. Default = 10.Valid range = [1, 20]\n"
        << "       --playertype <type>     Type of snake intelligence: random, backtracking. Default = backtracking.\n";
    return oss.str();
}

void snz::SnakeSimulation::print_welcome(void)
{
    std::cout << "\n-------------> Welcome to the classic Snaze Game <-------------\n"
              << "   copyright (C) 2023, Tobias dos Santos and Wisla Argolo.\n"
              << "---------------------------------------------------------------\n"
              << " Levels loaded: " << levels.size() << " | Snake lives: " << opt.lives << " | Foods to eat: " << opt.n_food_pellets_per_nivel << "\n"
              << " Clear all levels to win the game. Good luck!!!\n"
              << "---------------------------------------------------------------\n"
              << ">>> Press <ENTER> to start the game!\n\n";
}

void snz::SnakeSimulation::init_running_level()
{
    consumed_pellets = 0;
    snake.bind_level(&(*running_level));
    player.bind_level(&(*running_level));
    snake.init();

    running_level->place_pellet();
}

snz::SimulationResult snz::SnakeSimulation::initialize(int argc, char *argv[])
{
    bool file = false; 

    if (argc < 2)
        return {ERROR, options()};

    for (auto i{1}; i < argc; ++i)
    {
        std::string str = argv[i];

        for (auto j{0}; j < str.length(); ++j)
            str[j] = std::tolower(str[j]);

        if (str == "--help" || str == "--h")
        {
            return {ERROR, options()};
        }
        else if (str == "--fps")
        {
            if (i + 1 == argc)
            {
                std::cout << options();
                return {ERROR, ">>> ERROR! No FPS value provided.\n"};
            }

            try
            {
                opt.fps = std::stoi(argv[++i]);
            }
            catch (const std::exception &e)
            {
                return {ERROR, ">>> ERROR! Invalid value for FPS.\n"};
            }

            if (opt.fps < 1 || 50 < opt.fps)
                return {ERROR, ">>> ERROR! Invalid value for FPS. Valid range is [1,50]\n"};
        }
        else if (str == "--lives" || str == "--l")
        {
            if (i + 1 == argc)
            {
                std::cout << options();
                return {ERROR, ">>> ERROR! No lives value provided.\n"};
            }

            try
            {
                snake.set_lives(std::stoi(argv[++i]));
                opt.lives = std::stoi(argv[i]);
            }
            catch (const std::exception &e)
            {
                return {ERROR, ">>> ERROR! Invalid value for lives.\n"};
            }

            if (snake.get_lives() < 1 || 20 < snake.get_lives())
                return {ERROR, ">>> ERROR! Invalid value for lives. Valid range is [1,20]\n"};
        }
        else if (str == "--food")
        {
            if (i + 1 == argc)
            {
                std::cout << options();
                return {ERROR, ">>> ERROR! No food value provided.\n"};
            }

            try
            {
                opt.n_food_pellets_per_nivel = std::stoi(argv[++i]);
            }
            catch (const std::exception &e)
            {
                return {ERROR, ">>> ERROR! Invalid value for food.\n"};
            }

            if (opt.n_food_pellets_per_nivel < 1 || 20 < opt.n_food_pellets_per_nivel)
                return {ERROR, ">>> ERROR! Invalid value for food. Valid range is [1,20]\n"};
        }
        else if (str == "--playertype" || str == "--player" || str == "--p")
        {

            if (i + 1 == argc)
            {
                std::cout << options();
                return {ERROR, ">>> ERROR! No player type value provided.\n"};
            }

            std::string aux = argv[++i];
            for (auto j{0}; j < aux.length(); ++j)
                aux[j] = std::tolower(aux[j]);


            if(!(aux == "random" || aux == "backtracking")) {
                std::cout << options();
                return {ERROR, ">>> ERROR! Invalid value for player type.\n"};
            }

            opt.player_type = aux;
        }
        else {
            opt.file_name = str;
            file = true;
        }
    }

    if(!file) {
        std::cout << options();
        return {ERROR, ">>> ERROR! No file name provided.\n"};
    }

    try
    {
        read_levels();
    }
    catch (std::exception &e)
    {
        return {ERROR, ">>> ERROR! Read file problem: " + std::string(e.what())};
    }



    //if(opt.player_type == "backtracking") player = new BTSPlayer();
    //else player = new RandomSPlayer();

    state = simulation_state_e::START;
    player.bind_snake(&snake);

    return {OK, ">>> OK! Game initialized."};
}

void snz::SnakeSimulation::read_levels()
{
    std::ifstream file(opt.file_name);

    if (!file.is_open())
        throw std::runtime_error("Unable to open level file.\n");

    std::string line;

    while (std::getline(file, line))
    {

        if (!line.empty())
        {
            Level level;
            bool is_valid = false;

            std::istringstream ss(line);
            int r, c;

            if (!(ss >> r >> c) || r < 1 || c < 1 || c > 100 || r > 100)
                throw std::runtime_error("The values for number of rows and columns is invalid. Valid range is [1,100]\n");

            level.set_rows(r);
            level.set_cols(c);

            for (unsigned long i{0}; i < r; ++i)
            {
                std::getline(file, line);

                std::string aux;

                for (char c : line) {
                    if (c != '\r') aux += c;
                }

                line = aux;

                for (char c : line)
                {
                    if (c != '#' && c != '*' && c != '.' && c != ' ')
                        throw std::runtime_error("The file could not be read. It shows unexpected symbols.\n");
                }

                level.add_line(line);

                size_t spawn_pos = line.find('*');

                if (spawn_pos != std::string::npos)
                {
                    level.set_spawn({i, spawn_pos});
                    is_valid = true;
                }
            }

            if (is_valid)
                levels.push_back(level);
        }
    }

    if (levels.empty())
      throw std::runtime_error("No levels found in file.");

    running_level = levels.begin();

}

std::string snz::SnakeSimulation::repeat_string(const std::string s, int n)
{
    std::string repeated;
    for (int i{0}; i < n; ++i)
        repeated += s;

    return repeated;
}

void snz::SnakeSimulation::render_board()
{

    std::string symbols[] = {"✼", "󰯈", "", "", "", "", "●", "♥", "♡", "", "█", "◌"};
    // std::string symbols [] = {"✼", "☹", "<", ">", "∧", "∨", "●","♥", "♡", "⚜", "█", "◌"} // caso nao tenha a fonte instalada

    std::cout << "Lives: " << repeat_string(symbols[7], snake.get_lives()) << repeat_string(symbols[8], opt.lives - snake.get_lives())
              << " | Score: " << score << " | Food eaten: " << consumed_pellets
              << " of " << opt.n_food_pellets_per_nivel << " | Level: "
              << std::distance(levels.begin(), running_level) + 1 << " of " << levels.size() << "\n";
    std::cout << "------------------------------------------------------------------\n";

    size_t rows = running_level->get_rows();
    size_t cols = running_level->get_cols();

    for (size_t i{0}; i < rows; ++i)
    {
        for (size_t j{0}; j < cols; ++j)
        {
            tile_type_e type = running_level->get_tile_type({i, j});

            switch (type)
            {
            case tile_type_e::EMPTY:
                std::cout << " ";
                break;
            case tile_type_e::INVISIBLE:
                std::cout << " ";
                break;
            case tile_type_e::WALL:
                std::cout << symbols[10];
                break;
            case tile_type_e::SNAKEHEAD:
                if (state == simulation_state_e::START)
                    std::cout << symbols[0];
                else if (state == simulation_state_e::CRASH || (state == simulation_state_e::GAME_OVER && snake.get_lives() == 0))
                    std::cout << symbols[1];
                else
                {
                    if (snake.get_dir().dy == 1)
                        std::cout << symbols[4];
                    else if (snake.get_dir().dy == -1)
                        std::cout << symbols[5];
                    else if (snake.get_dir().dx == 1)
                        std::cout << symbols[2];
                    else if (snake.get_dir().dx == -1)
                        std::cout << symbols[3];
                    else if(snake.get_dir() == MoveDir{0,0})
                        std::cout << symbols[0];
                }
                break;
            case tile_type_e::SNAKEBODY:
                if (state == simulation_state_e::CRASH) 
                    std::cout << symbols[11];
                else
                    std::cout << symbols[6];
                break;
            case tile_type_e::FOOD:
                std::cout << symbols[9];
                break;
            }
        }
        std::cout << std::endl;
    }
}

void snz::SnakeSimulation::print_level_up()
{
    std::cout << "\nYeah! The snake advanced to the next level!\n"
              << ">>> Press <ENTER> to start the next level.\n"
              << "---------------------------------------------------------------\n\n";
}

void snz::SnakeSimulation::print_crashed()
{
    std::cout << "\nOh no! The snake crashed!\n"
              << ">>> Press <ENTER> to try again.\n"
              << "---------------------------------------------------------------\n\n";
}

void snz::SnakeSimulation::print_won()
{
    std::cout << "\n+-------------------------------------+\n"
              << "|       CONGRATS! The snake WON!      |\n"
              << "|         Thanks for playing!         |\n"
              << "+-------------------------------------+\n";
}

void snz::SnakeSimulation::print_lost()
{
    std::cout << "\n+-------------------------------------+\n"
              << "|        OH NO! The snake LOSE!       |\n"
              << "|         Thanks for playing!         |\n"
              << "+-------------------------------------+\n";
}

void snz::SnakeSimulation::process_events()
{ // INCOMPLETO

    if (state == simulation_state_e::START ||
        state == simulation_state_e::CRASH ||
        state == simulation_state_e::LEVEL_UP)
        std::cin.get();
}

void snz::SnakeSimulation::update()
{

    if (state == simulation_state_e::START)
    {
        init_running_level();
        state = simulation_state_e::THINKING;
    }
    else if (state == simulation_state_e::LEVEL_UP)
    {
        ++running_level;
        init_running_level(); // começa outro
        state = simulation_state_e::THINKING;
        
    }
    else if (state == simulation_state_e::CRASH)
    {
        for (const auto &tile_pos : snake.get_body())
            running_level->set_tile_type(tile_pos, tile_type_e::EMPTY);

        snake.init();

        state = simulation_state_e::THINKING; 
    }
    else if (state == simulation_state_e::RUN)
    {
        MoveDir new_dir = player.next_move_random();
        if(opt.player_type == "backtracking") {
            if(!player.get_solution().empty()) new_dir = player.next_move(); 
        } 
     
        snake.set_dir(new_dir);

        if (running_level->is_food(snake.get_next_location(new_dir)))
        {
            ++consumed_pellets;
            score += 10 + snake.get_body().size()*5;
            
            if (consumed_pellets == opt.n_food_pellets_per_nivel)
            {
                snake.step_foward(new_dir);
                if(running_level == levels.end() - 1) 
                    state = simulation_state_e::GAME_OVER;
                else
                    state = simulation_state_e::LEVEL_UP;
                return;
            }
            running_level->place_pellet();
            state = simulation_state_e::THINKING;

        } else if (snake.get_next_location(new_dir) != snake.get_body().back() && running_level->crashed(snake.get_next_location(new_dir))) {
            snake.set_lives(snake.get_lives() - 1);
            if (snake.get_lives() == 0) state = simulation_state_e::GAME_OVER;
            else state = simulation_state_e::CRASH;
            return;
        
        }

        snake.step_foward(new_dir);
    } else if(state == simulation_state_e::THINKING) {
        if(opt.player_type == "backtracking")
            player.find_solution();
        state = simulation_state_e::RUN;
    }
}

void snz::SnakeSimulation::render()
{

    if(state == simulation_state_e::THINKING) 
        return ;

    std::chrono::milliseconds duration{1000 / opt.fps};
    std::this_thread::sleep_for(duration);

    std::system("clear");

    if (state == simulation_state_e::START)
        print_welcome();
    else if (state == simulation_state_e::LEVEL_UP)
        print_level_up();
    else if (state == simulation_state_e::CRASH) {
        print_crashed();
    } else if (state == simulation_state_e::GAME_OVER) {
        if(snake.get_lives() == 0) print_lost();
        else print_won();
       // return ;
    }

    render_board();
}

bool snz::SnakeSimulation::game_over()
{
    return (state == simulation_state_e::GAME_OVER);
}