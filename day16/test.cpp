#include "lib.h"
#include <gtest/gtest.h>

TEST(GetCost, Correct) {
  EXPECT_EQ(get_cost(Direction::Right, Direction::Right), 0);
  EXPECT_EQ(get_cost(Direction::Right, Direction::Up), 1000);
  EXPECT_EQ(get_cost(Direction::Right, Direction::Down), 1000);
  EXPECT_EQ(get_cost(Direction::Left, Direction::Right), 2000);
}

TEST(Examples, Example1) {
  const std::string data = "###############\n"
                           "#.......#....E#\n"
                           "#.#.###.#.###.#\n"
                           "#.....#.#...#.#\n"
                           "#.###.#####.#.#\n"
                           "#.#.#.......#.#\n"
                           "#.#.#####.###.#\n"
                           "#...........#.#\n"
                           "###.#.#####.#.#\n"
                           "#...#.....#.#.#\n"
                           "#.#.#.###.#.#.#\n"
                           "#.....#...#.#.#\n"
                           "#.###.#.#.#.#.#\n"
                           "#S..#.....#...#\n"
                           "###############\n";

  std::istringstream stream(data);
  const Map map = read_data(stream);

  auto map_path = map.get_path();
  EXPECT_EQ(map_path.score, 7036);

  std::ostringstream path;
  path << map_path;

  const std::string expected = "###############\n"
                               "#.......#....E#\n"
                               "#.#.###.#.###x#\n"
                               "#.....#.#...#x#\n"
                               "#.###.#####.#x#\n"
                               "#.#.#.......#x#\n"
                               "#.#.#####.###x#\n"
                               "#..xxxxxxxxx#x#\n"
                               "###x#.#####x#x#\n"
                               "#xxx#.....#x#x#\n"
                               "#x#.#.###.#x#x#\n"
                               "#x....#...#x#x#\n"
                               "#x###.#.#.#x#x#\n"
                               "#S..#.....#xxx#\n"
                               "###############\n";

  EXPECT_EQ(path.str(), expected);
}
