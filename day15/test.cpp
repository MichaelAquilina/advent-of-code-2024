#include "lib.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Examples, SmallExample) {
  std::string data = "########\n"
                     "#..O.O.#\n"
                     "##@.O..#\n"
                     "#...O..#\n"
                     "#.#.O..#\n"
                     "#...O..#\n"
                     "#......#\n"
                     "########\n"
                     "\n"
                     "<^^>>>vv<v>>v<<\n";

  std::stringstream stream(data);
  auto [map, directions] = read_data(stream);

  Point start = {2, 2};
  EXPECT_EQ(map.robot, start);

  const auto part1 = get_part1(map, directions);
  EXPECT_EQ(part1, 2028);
}

TEST(Examples, LargeExample) {
  std::string data =
      "##########\n"
      "#..O..O.O#\n"
      "#......O.#\n"
      "#.OO..O.O#\n"
      "#..O@..O.#\n"
      "#O#..O...#\n"
      "#O..O..O.#\n"
      "#.OO.O.OO#\n"
      "#....O...#\n"
      "##########\n"
      "\n"
      "<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^\n"
      "vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v\n"
      "><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<\n"
      "<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^\n"
      "^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><\n"
      "^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^\n"
      ">^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^\n"
      "<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>\n"
      "^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>\n"
      "v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^"
      "\n";

  std::istringstream stream(data);
  auto [map, directions] = read_data(stream);

  const Point expected_start = Point{4, 4};
  EXPECT_EQ(map.robot, expected_start);

  const auto part1 = get_part1(map, directions);
  EXPECT_EQ(part1, 10092);

  std::stringstream output;
  output << map;

  std::string expected_output = "##########\n"
                                "#.O.O.OOO#\n"
                                "#........#\n"
                                "#OO......#\n"
                                "#OO@.....#\n"
                                "#O#.....O#\n"
                                "#O.....OO#\n"
                                "#O.....OO#\n"
                                "#OO....OO#\n"
                                "##########\n";

  EXPECT_EQ(output.str(), expected_output);
}
