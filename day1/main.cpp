#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <unordered_map>
#include <vector>

struct Distances {
  std::vector<int> left;
  std::vector<int> right;
};

Distances load_input_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file.is_open()) {
    throw std::runtime_error("Unable to read");
  }

  Distances distances;
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream stream(line);
    int token1, token2;

    if (stream >> token1 >> token2) {
      distances.left.push_back(token1);
      distances.right.push_back(token2);
    } else {
      throw std::runtime_error("unable to read expected data from file");
    }
  }
  file.close();

  std::sort(distances.left.begin(), distances.left.end());
  std::sort(distances.right.begin(), distances.right.end());

  return distances;
}

uint get_part1(const Distances &data) {
  std::vector<uint> results;

  auto left = data.left.begin();
  auto right = data.right.begin();

  while (left != data.left.end() && right != data.right.end()) {
    const uint distance = std::abs(*left - *right);
    results.push_back(distance);

    ++right;
    ++left;
  }

  return std::accumulate(results.begin(), results.end(), 0);
}

uint get_part2(const Distances &data) {
  uint result = 0;

  std::unordered_map<uint, uint> right_counter;
  for (const auto &item : data.right) {
    right_counter[item] += 1;
  }

  for (const auto &item : data.left) {
    const auto count = right_counter.find(item);
    if (count != right_counter.end()) {
      result += count->second * item;
    }
  }

  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  if (args.size() != 2) {
    throw std::runtime_error("Expected path as argument");
  }

  const std::filesystem::path path = args[1];

  const auto data = load_input_data(path);
  const auto part1 = get_part1(data);
  const auto part2 = get_part2(data);

  std::cout << "Part 1: " << part1 << std::endl;
  std::cout << "Part 2: " << part2 << std::endl;

  return 0;
}
