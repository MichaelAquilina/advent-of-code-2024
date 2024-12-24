#include "lib.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>

Map read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Unable to open file");
  }

  std::string line;
  std::vector<std::vector<char>> points;
  uint width = 0;
  uint height = 0;

  while (std::getline(file, line)) {
    width = line.size();
    std::vector<char> row;
    for (uint x = 0; x < line.size(); x++) {
      for (char &crop : line) {
        row.push_back(crop);
      }
    }
    points.push_back(row);
    height++;
  }

  return Map(points, width, height);
}

std::tuple<uint, uint> get_parts(const std::vector<Fence> &fences,
                                 bool debug_part1, bool debug_part2) {
  uint part1 = 0;
  uint part2 = 0;
  for (auto &fence : fences) {
    uint area = fence.get_area();
    uint perimeter = fence.get_perimeter_length();
    uint sides = fence.get_sides();
    uint cost1 = area * perimeter;
    uint cost2 = sides * area;

    if (debug_part1) {
      std::cout << "A region of " << fence.value << " plants with price "
                << area << " * " << perimeter << " = " << cost1 << ".\n";
    }

    if (debug_part2) {
      std::cout << "A region of " << fence.value << " plants with price "
                << area << " * " << sides << " = " << cost2 << ".\n";
    }

    part1 += cost1;
    part2 += cost2;
  }
  return {part1, part2};
}
