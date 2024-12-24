#include "lib.h"
#include <filesystem>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argc + argv);

  if (args.size() < 2) {
    throw std::runtime_error("Must provde path to input.txt");
  }

  const std::filesystem::path path = args[1];

  bool debug_part1 = false;
  bool debug_part2 = false;
  if (args.size() >= 3) {
    const auto &flag = args.at(2);
    if (flag == "--debug-part1") {
      debug_part1 = true;
    } else if (flag == "--debug-part2") {
      debug_part2 = true;
    }
  }

  const Map map = read_data(path);

  const auto fences = map.get_fences();

  const auto [part1, part2] = get_parts(fences, debug_part1, debug_part2);

  std::cout << "Part 1: " << part1 << std::endl;
  std::cout << "Part 2: " << part2 << std::endl;
}
