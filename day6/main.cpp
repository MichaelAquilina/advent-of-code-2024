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

  auto map = read_data(path);

  auto [width, height] = map.get_dimensions();
  auto [start, direction] = map.get_current_position();

  std::cout << "Map size: " << width << "x" << height << std::endl;
  std::cout << "Start: " << start << std::endl;

  auto visited = run_to_end(map, start, direction, debug);

  // -1 because the last one is out of bounds
  std::cout << "Part 1: " << visited.size() - 1 << std::endl;

  // brute force all visited locations to see which introductions
  // introduce loops. Is there anything smarter than that?
}
