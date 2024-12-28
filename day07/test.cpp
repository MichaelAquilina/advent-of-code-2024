#include "lib.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(Part1, calculates_correctly) {
  std::vector<unsigned long> operands = {81, 40, 27};
  std::vector<Operator> operators = {Operator::Add, Operator::Multiply};
  const auto result = calculate(operands, operators);
  EXPECT_EQ(result, 3267);
}

TEST(Part2, calculates_correctly) {
  std::vector<unsigned long> operands = {17, 8, 14};
  std::vector<Operator> operators = {Operator::Concat, Operator::Add};
  const auto result = calculate(operands, operators);
  EXPECT_EQ(result, 192);
}

TEST(Part1, generate_pemutations_1) {
  std::vector<Operator> options = {Operator::Add, Operator::Multiply};
  const auto result = generate_permutations(1, options);

  const std::vector<std::vector<Operator>> expected = {{Operator::Add},
                                                       {Operator::Multiply}};
  EXPECT_EQ(result, expected);
}

TEST(Part1, generate_permutations_2) {
  std::vector<Operator> options = {Operator::Add, Operator::Multiply};
  const auto result = generate_permutations(2, options);

  const std::vector<std::vector<Operator>> expected = {
      {Operator::Add, Operator::Add},
      {Operator::Multiply, Operator::Add},
      {Operator::Add, Operator::Multiply},
      {Operator::Multiply, Operator::Multiply},
  };
  EXPECT_EQ(result, expected);
}
