#include "lib.h"
#include <filesystem>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argc + argv);

  if (args.size() != 2) {
    throw std::runtime_error("Must provde path to input.txt");
  }

  const std::filesystem::path path = args[1];

  const auto equations = read_data(path);

  std::vector<Operator> part1_operators = {Operator::Add, Operator::Multiply};
  const auto part1 = check_matches(equations, part1_operators);

  std::cout << "Part 1: " << part1 << std::endl;

  std::vector<Operator> part2_operators = {Operator::Add, Operator::Multiply,
                                           Operator::Concat};
  const auto part2 = check_matches(equations, part2_operators);

  std::cout << "Part 2: " << part2 << std::endl;
}
