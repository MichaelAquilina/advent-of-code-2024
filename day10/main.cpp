#include "lib.h"
#include <filesystem>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argc + argv);

  if (args.size() != 2) {
    throw std::runtime_error("Must provde path to input.txt");
  }

  const std::filesystem::path path = args[1];

  const auto map = read_data(path);
  const auto [width, height] = map.get_dimensions();

  std::cout << width << "x" << height << std::endl;

  const auto part1 = map.traverse();

  std::cout << "Part 1: " << part1 << std::endl;

}
