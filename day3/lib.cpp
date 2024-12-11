#include "lib.h"
#include <filesystem>
#include <fstream>
#include <iterator>
#include <regex>
#include <vector>

std::vector<instruction> read_data(const std::filesystem::path &path) {
  std::vector<instruction> results;

  std::ifstream file(path);
  std::string text{std::istreambuf_iterator<char>(file),
                   std::istreambuf_iterator<char>()};

  std::regex pattern(R"((mul|do|don't)\(((\d+),(\d+))?\))");

  auto begin = std::sregex_iterator(text.begin(), text.end(), pattern);
  auto end = std::sregex_iterator();

  for (auto it = begin; it != end; ++it) {
    std::smatch match = *it;

    instruction item;
    if (match[1] == "mul") {
      item = {InstructionType::Mul, std::stoi(match[3]), std::stoi(match[4])};
    } else if (match[1] == "do") {
      item = {InstructionType::Do, 0, 0};
    } else if (match[1] == "don't") {
      item = {InstructionType::Dont, 0, 0};
    }
    results.push_back(item);
  }

  return results;
}

uint calculate(const std::vector<instruction> &data, bool dodont) {
  uint total = 0;
  bool active = true;

  for (auto &item : data) {
    if (item.instruction_type == InstructionType::Mul && (!dodont || active)) {
      total += (item.arg1 * item.arg2);
    } else if (item.instruction_type == InstructionType::Do) {
      active = true;
    } else if (item.instruction_type == InstructionType::Dont) {
      active = false;
    }
  }

  return total;
}
