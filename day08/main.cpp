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

  Map map = read_data(path);
  auto [width, height] = map.get_dimensions();
  auto count = map.get_antenna_count();

  std::cout << width << "x" << height << std::endl;
  std::cout << count << std::endl;

  auto part1 = get_antinodes(map);
  std::cout << "Part 1: " << part1.size() << std::endl;

  auto part2 = get_antinodes(map, true);

  print_map(map, part2);
  std::cout << "Part 2: " << part2.size() << std::endl;
}
