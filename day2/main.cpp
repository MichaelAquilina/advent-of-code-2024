#include <filesystem>
#include <iostream>
#include <vector>
#include "lib.h"

int main(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argc + argv);

  if (args.size() != 2) {
    throw std::runtime_error("Must provde path to input.txt");
  }

  const std::filesystem::path path = args[1];

  const auto reports = read_data(path);

  const auto part1 = get_part1(reports);
  std::cout << "Part 1: " << part1 << std::endl;

  const auto part2 = get_part2(reports);
  std::cout << "Part 2: " << part2 << std::endl;
}
