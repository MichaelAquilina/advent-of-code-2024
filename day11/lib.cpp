#include "lib.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

std::vector<unsigned long> read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("File could not be open");
  }

  std::string line;
  std::vector<unsigned long> stones;

  while (std::getline(file, line)) {
    std::istringstream stream(line);

    uint stone;
    while (true) {
      stream >> stone;
      if (!stream) {
        break;
      }

      stones.push_back(stone);
    }
  }

  return stones;
}

std::vector<unsigned long> blink(std::vector<unsigned long> &stones,
                                 uint count) {
  std::vector<unsigned long> &current = stones;
  for (uint i = 0; i < count; i++) {
    std::vector<unsigned long> next;
    for (auto &stone : stones) {
      if (stone == 0) {
        next.push_back(1);
        continue;
      } else {
        std::string digits = std::to_string(stone);
        if (digits.size() % 2 == 0) {
          unsigned long left = stoul(digits.substr(0, digits.size() / 2));
          unsigned long right =
              stoul(digits.substr(digits.size() / 2, digits.size()));
          next.push_back(left);
          next.push_back(right);
        } else {
          next.push_back(stone * 2024);
        }
      }
    }
    current = next;
  }
  return current;
}

void print_stones(std::vector<unsigned long> &stones) {
  for (auto &stone : stones) {
    std::cout << stone << " ";
  }
  std::cout << std::endl;
}

// takes a while but works
unsigned long blink_many(std::vector<unsigned long> &stones, uint times) {
  unsigned long total = 0;
  uint batch = 25;

  std::unordered_map<unsigned long, std::vector<unsigned long>> cache;

  // better would be to store <stone, count> as key for better caching
  std::unordered_map<unsigned long, unsigned long> secondary_cache;

  std::vector<std::tuple<unsigned long, uint>> stack;
  for (const auto &stone : stones) {
    stack.push_back({stone, times});
  }

  unsigned long i = 0;

  while (!stack.empty()) {
    if (i % 1000000 == 0) {
      std::cout << i << " : " << stack.size() << " : " << total << std::endl;
    }
    i++;

    auto [stone, remaining] = stack.back();
    stack.pop_back();
    uint steps = std::min(remaining, batch);

    std::vector<unsigned long> new_stones;
    if (remaining == batch && secondary_cache.contains(stone)) {
      total += secondary_cache.at(stone);
      continue;
    }

    if (steps == batch && cache.contains(stone)) {
      new_stones = cache.at(stone);
    } else {
      std::vector<unsigned long> input = {stone};
      new_stones = blink(input, steps);
    }

    if (remaining <= batch) {
      total += new_stones.size();
      if (remaining == batch) {
        secondary_cache[stone] = new_stones.size();
      }
    } else {
      const uint new_remaining = remaining - batch;
      for (auto &item : new_stones) {
        stack.push_back({item, new_remaining});
      }
    }
  }
  return total;
}
