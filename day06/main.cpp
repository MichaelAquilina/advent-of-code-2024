#include "lib.h"
#include <filesystem>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argc + argv);
  bool debug = false;

  if (args.size() < 2) {
    throw std::runtime_error("Must provde path to input.txt");
  }

  if (args.size() > 2 && args[2] == "--debug") {
    debug = true;
  }

  const std::filesystem::path path = args[1];

  auto [map, start, direction] = read_data(path);

  auto [visited, _] = run_to_end(map, start, direction, debug);

  // -1 because the last one is out of bounds
  std::cout << "Part 1: " << visited.size() - 1 << std::endl;

  const auto loop_points =
      find_loop_points(map, visited, start, direction, debug);

  std::cout << "Part 2: " << loop_points.size() << std::endl;
}
