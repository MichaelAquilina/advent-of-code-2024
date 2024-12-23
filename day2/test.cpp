#include "lib.h"
#include <gtest/gtest.h>

TEST(Part1, TestExample) {
  const std::vector<std::vector<uint>> reports = {
      {7, 6, 4, 2, 1}, {1, 2, 7, 8, 9}, {9, 7, 6, 2, 1},
      {1, 3, 2, 4, 5}, {8, 6, 4, 4, 1}, {1, 3, 6, 7, 9},
  };

  EXPECT_EQ(get_part1(reports), 2);
}

TEST(Part2, TestExample) {
  const std::vector<std::vector<uint>> reports = {
      {7, 6, 4, 2, 1}, {1, 2, 7, 8, 9}, {9, 7, 6, 2, 1},
      {1, 3, 2, 4, 5}, {8, 6, 4, 4, 1}, {1, 3, 6, 7, 9},
  };

  EXPECT_EQ(get_part2(reports), 4);
}

TEST(TestIsSafe, EmptyIsSafe) { EXPECT_TRUE(is_safe({})); }

TEST(TestIsSafe, SingleItemIsSafe) { EXPECT_TRUE(is_safe({1})); }

TEST(TestIsSafe, IncreasingSafe) {
  EXPECT_TRUE(is_safe({1, 2, 3, 4, 5, 6, 7}));
  EXPECT_TRUE(is_safe({8, 11, 14, 17, 20}));
  EXPECT_TRUE(is_safe({8, 9, 11, 14, 15}));
}

TEST(TestIsSafe, DecreasingSafe) {
  EXPECT_TRUE(is_safe({8, 7, 6, 5, 4, 3, 2, 1}));
  EXPECT_TRUE(is_safe({8, 5, 2}));
  EXPECT_TRUE(is_safe({20, 18, 17, 14, 13, 10}));
}

TEST(TestIsSafe, SameValueNotSafe) {
  EXPECT_FALSE(is_safe({1, 2, 3, 4, 4}));
  EXPECT_FALSE(is_safe({2, 3, 3, 4, 5}));
}

TEST(TestIsSafe, ChangeNotSafe) {
  EXPECT_FALSE(is_safe({1, 2, 3, 4, 3, 4}));
  EXPECT_FALSE(is_safe({10, 9, 8, 9, 10}));
}

TEST(TestIsSafeTolerated, ToleratedSafe) {
  EXPECT_TRUE(is_safe({1, 2, 3, 2, 5, 6}, 1));
  EXPECT_TRUE(is_safe({10, 9, 8, 9, 7}, 1));
  EXPECT_TRUE(is_safe({1, 10, 2, 3, 4, 5}, 1));
  EXPECT_TRUE(is_safe({10, 1, 2, 3, 4}, 1));
  // this is the case that is failing, when we can
  // remove the first element to fix increasing/decreasing
  EXPECT_TRUE(is_safe({3, 1, 2, 3, 4}, 1));
  EXPECT_TRUE(is_safe({1, 2, 3, 4, 1}, 1));
  EXPECT_TRUE(is_safe({1, 2, 3, 4, 10}, 1));
}
