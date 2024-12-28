#include "lib.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<std::vector<uint>> read_data(const std::filesystem::path &path) {
  std::vector<std::vector<uint>> results;

  std::ifstream input_file(path);

  if (!input_file.is_open()) {
    throw std::runtime_error("Unable to read from input file");
  }

  std::string line;

  while (std::getline(input_file, line)) {
    std::istringstream stream(line);
    std::vector<uint> report;
    uint token;

    while (stream >> token) {
      report.push_back(token);
    }
    results.push_back(report);
  }

  return results;
}

uint check_safety(const int t0, const int t1, const int difference) {
  const int current_difference = t1 - t0;

  // difference should only ever be 0 at the start
  // other loops we are guaranteed to have a value between 1 and 3
  // check that both positive or both negative
  if (difference == 0 || difference * current_difference > 0) {
    const uint abs_difference = std::abs(current_difference);
    if (abs_difference < 1 || abs_difference > 3) {
      return 0;
    }
  } else {
    return 0;
  }

  return current_difference;
}

bool is_safe(std::vector<uint> report, int tolerate) {
  int difference = 0;

  if (report.size() == 0) {
    return true;
  }

  for (uint i = 0; i < report.size() - 1; i++) {
    const uint t0 = report[i];
    const uint t1 = report[i + 1];

    const uint current_difference = check_safety(t0, t1, difference);
    if (current_difference == 0) {
      if (tolerate > 0) {
        // this is quite a hacky implementation
        // maybe its nicer just to try all permutations of this vector
        // with one element removed instead?
        std::vector<uint> report1 = report;
        std::vector<uint> report2 = report;
        report1.erase(report1.begin() + i);
        report2.erase(report2.begin() + i + 1);

        bool report3_safety = false;
        if (i > 0) {
          std::vector<uint> report3 = report;
          report3.erase(report3.begin() + i - 1);
          report3_safety = is_safe(report3, tolerate - 1);
        }

        return is_safe(report1, tolerate - 1) ||
               is_safe(report2, tolerate - 1) || report3_safety;
      } else {
        return false;
      }
    }

    difference = current_difference;
  }
  return true;
}

uint get_part1(const std::vector<std::vector<uint>> &reports) {
  uint result = 0;
  for (const auto &report : reports) {
    if (is_safe(report)) {
      result++;
    }
  }

  return result;
}

uint get_part2(const std::vector<std::vector<uint>> &reports) {
  uint result = 0;
  for (const auto &report : reports) {
    if (is_safe(report, 1)) {
      result++;
    } else {
    }
  }

  return result;
}
