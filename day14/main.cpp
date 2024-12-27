#include "lib.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argc + argv);

  if (args.size() < 2) {
    throw std::runtime_error("Must provde path to input.txt");
  }

  uint width = 101;
  uint height = 103;
  if (args.size() == 4) {
    width = std::stoul(args.at(2));
    height = std::stoul(args.at(3));
  }

  const std::filesystem::path path = args.at(1);

  auto robots = read_data(path);

  const auto part1 = get_part1(robots, width, height);

  std::cout << "Part 1: " << part1 << "\n";

  get_part2(robots, width, height);
}
