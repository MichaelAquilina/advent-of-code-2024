#include "lib.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

std::vector<std::vector<char>> read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Unable to read file");
  }

  std::vector<std::vector<char>> output;

  int index = 0;
  std::string line;
  while (std::getline(file, line)) {
    output.push_back({});
    for (const char item : line) {
      output.at(index).push_back(item);
    }
    index++;
  }

  return output;
}

bool check_target_direction(int x, int y, int x_vector, int y_vector,
                            const std::string &target,
                            const std::vector<std::vector<char>> &data) {
  if (x_vector == 0 && y_vector == 0) {
    return false;
  }

  uint index = 0;

  while (index < target.size()) {
    const int x_delta = x_vector * index;
    const int y_delta = y_vector * index;

    if (x + x_delta < 0 || y + y_delta < 0) {
      return false;
    }

    const uint x_coord = x + x_delta;
    const uint y_coord = y + y_delta;

    if (x_coord >= data.size()) {
      return false;
    }

    if (y_coord >= data.at(x).size()) {
      return false;
    }

    const auto &item = data.at(x_coord).at(y_coord);
    if (item != target.at(index)) {
      return false;
    }
    index++;
  }

  return true;
}

uint check_xmas(int x, int y, const std::vector<std::vector<char>> &data) {
  uint count = 0;
  const std::string target = "XMAS";

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (check_target_direction(x, y, i, j, target, data)) {
        count++;
      }
    }
  }
  return count;
}

bool check_cross_mas(int x, int y, const std::vector<std::vector<char>> &data) {
  const std::string target = "MAS";

  const std::vector<std::tuple<int, int>> pairs = {
      {-1, -1},
      {1, -1},
      {1, 1},
      {-1, 1},
  };

  for (uint i = 0; i < pairs.size(); i++) {
    const auto &[x_0, y_0] = pairs.at(i);
    const auto &[x_1, y_1] = pairs.at((i + 1) % pairs.size());

    if (check_target_direction(x + x_0, y + y_0, -x_0, -y_0, target, data) &&
        check_target_direction(x + x_1, y + y_1, -x_1, -y_1, target, data)) {
      return true;
    }
  }
  return false;
}

uint get_part2(const std::vector<std::vector<char>> &data) {
  uint count = 0;

  for (uint x = 0; x < data.size(); x++) {
    const auto &row = data.at(x);
    for (uint y = 0; y < row.size(); y++) {
      const auto &item = row.at(y);
      if (item == 'A' && check_cross_mas(x, y, data)) {
        count++;
      }
    }
  }
  return count;
}

uint get_part1(const std::vector<std::vector<char>> &data) {
  uint count = 0;

  for (uint x = 0; x < data.size(); x++) {
    const auto &row = data.at(x);
    for (uint y = 0; y < row.size(); y++) {
      const auto &item = row.at(y);
      if (item == 'X') {
        count += check_xmas(x, y, data);
      }
    }
  }
  return count;
}
