#include "lib.h"
#include "absl/strings/str_split.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

std::vector<Equation> read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Unable to read file");
  }

  std::string line;
  std::vector<Equation> equations;

  while (std::getline(file, line)) {
    std::vector<std::string> tokens = absl::StrSplit(line, ": ");
    auto total = stoul(tokens.at(0));

    tokens = absl::StrSplit(tokens.at(1), " ");
    std::vector<unsigned long> operators;

    for (std::string op : tokens) {
      operators.push_back(std::stoul(op));
    }

    equations.push_back(Equation{total, operators});
  }

  return equations;
}

unsigned long calculate(const std::vector<unsigned long> &operands,
                        const std::vector<Operator> &operators) {
  assert(operators.size() == operands.size() - 1);

  unsigned long total = operands.at(0);

  for (uint i = 0; i < operators.size(); i++) {
    auto rhs = operands.at(i + 1);
    auto op = operators.at(i);

    switch (op) {
    case Operator::Add:
      total += rhs;
      break;
    case Operator::Multiply:
      total *= rhs;
      break;
    case Operator::Concat:
      total = std::stoul(std::to_string(total) + std::to_string(rhs));
      break;
    }
  }
  return total;
}

std::vector<std::vector<Operator>>
generate_permutations(uint size, std::vector<Operator> &options) {
  uint base = options.size();
  std::vector<std::vector<Operator>> permutations(std::pow(base, size));

  for (uint i = 0; i < size; i++) {
    for (uint j = 0; j < permutations.size(); j++) {
      int index = std::floor(j / std::pow(base, i));
      permutations.at(j).push_back(options.at(index % base));
    }
  }
  return permutations;
}

unsigned long check_matches(const std::vector<Equation> &equations,
                            std::vector<Operator> &operators) {
  unsigned long total = 0;
  for (auto &equation : equations) {
    auto permutations =
        generate_permutations(equation.operands.size() - 1, operators);
    for (auto &operators : permutations) {
      auto result = calculate(equation.operands, operators);
      if (result == equation.total) {
        total += result;
        break;
      }
    }
  }

  return total;
}
