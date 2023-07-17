/**!
 *  This program implements the Snaze simulation.
 *
 *  @author Selan R. dos Santos
 *
 *  To compile: use cmake script.
 */

#include <cstdlib> // EXIT_SUCCESS

#include "../include/simulation.h"

int main(int argc, char *argv[])
{
  snz::SnakeSimulation simulation;

  auto result = simulation.initialize(argc, argv);
  if (result.type == snz::simulation_result_e::ERROR)
  {
    std::cerr << result.msg << "\n";
    return EXIT_FAILURE;
  }

  // Initial message.
  simulation.render();
  // The Game Loop.
  while (not simulation.game_over())
  {
    simulation.process_events();
    simulation.update();
    simulation.render();
  }

  return EXIT_SUCCESS;
}
