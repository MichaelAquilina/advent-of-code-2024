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

  auto stones = read_data(path);

  const auto part1 = blink_many(stones);

  std::cout << "Part 1: " << part1 << std::endl;

  const auto part2 = blink_many(stones, 75);

  std::cout << "Part 2: " << part2 << std::endl;
}
