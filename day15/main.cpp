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

  const std::filesystem::path path = args.at(1);

  auto [map, directions] = read_data(path);

  auto part1 = get_part1(map, directions);

  std::cout << "Part 1: " << part1 << "\n";
}
