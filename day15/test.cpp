#include "lib.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>

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

  EXPECT_EQ(map.robot, Point(2, 2));

  const auto part1 = get_part(map, directions);
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
  auto scaled_map = scale_up(map);

  EXPECT_EQ(map.robot, Point(4, 4));

  const auto part1 = get_part(map, directions);
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

  const auto part2 = get_part(scaled_map, directions);
  EXPECT_EQ(part2, 9021);

  output.str("");
  output << scaled_map;

  expected_output = "####################\n"
                    "##[].......[].[][]##\n"
                    "##[]...........[].##\n"
                    "##[]........[][][]##\n"
                    "##[]......[]....[]##\n"
                    "##..##......[]....##\n"
                    "##..[]............##\n"
                    "##..@......[].[][]##\n"
                    "##......[][]..[]..##\n"
                    "####################\n";

  EXPECT_EQ(output.str(), expected_output);
}

TEST(Map, ScaleUp) {
  const std::string data = "#######\n"
                           "#...#.#\n"
                           "#.....#\n"
                           "#..OO@#\n"
                           "#..O..#\n"
                           "#.....#\n"
                           "#######\n"
                           "\n"
                           "<vv<<^^<<^^\n";

  std::istringstream stream(data);
  auto [map, directions] = read_data(stream);

  const Map scaled_map = scale_up(map);

  EXPECT_EQ(scaled_map.robot, Point(10, 3));

  std::ostringstream output;
  output << scaled_map;

  const std::string expected = "##############\n"
                               "##......##..##\n"
                               "##..........##\n"
                               "##....[][]@.##\n"
                               "##....[]....##\n"
                               "##..........##\n"
                               "##############\n";

  EXPECT_EQ(output.str(), expected);
}

TEST(Map, ScaledUpHorizontalMovement) {
  const std::string data = "##############\n"
                           "##...#..##..##\n"
                           "##....[]....##\n"
                           "##...[][]@..##\n"
                           "##....[]....##\n"
                           "##..........##\n"
                           "##############\n"
                           "\n\n";

  std::istringstream stream(data);
  auto [map, _] = read_data(stream);

  map.move(Direction::Left);

  std::string expected = "##############\n"
                         "##...#..##..##\n"
                         "##....[]....##\n"
                         "##..[][]@...##\n"
                         "##....[]....##\n"
                         "##..........##\n"
                         "##############\n";

  std::stringstream output;
  output << map;
  EXPECT_EQ(output.str(), expected);

  map.robot = Point(3, 3);
  map.move(Direction::Right);
  map.move(Direction::Right);

  expected = "##############\n"
             "##...#..##..##\n"
             "##....[]....##\n"
             "##...@[][]..##\n"
             "##....[]....##\n"
             "##..........##\n"
             "##############\n";

  output.str("");
  output << map;
  EXPECT_EQ(output.str(), expected);
}

TEST(Map, ScaledUpVerticalMovement) {
  const std::string data = "##############\n"
                           "##...#..##..##\n"
                           "##....[]....##\n"
                           "##...[][][].##\n"
                           "##....[]..[]##\n"
                           "##.....@....##\n"
                           "##############\n"
                           "\n\n";

  std::istringstream stream(data);
  auto [map, _] = read_data(stream);

  map.move(Direction::Up);

  std::string expected = "##############\n"
                         "##...#[]##..##\n"
                         "##...[][]...##\n"
                         "##....[].[].##\n"
                         "##.....@..[]##\n"
                         "##..........##\n"
                         "##############\n";

  std::ostringstream output;
  output << map;

  EXPECT_EQ(output.str(), expected);

  output.str("");
  map.move(Direction::Up);
  output << map;

  // should be in the same spot since it hit a wall
  EXPECT_EQ(output.str(), expected);

  map.robot = Point(10, 2);
  map.move(Direction::Down);

  expected = "##############\n"
             "##...#[]##..##\n"
             "##...[][]...##\n"
             "##....[]..@.##\n"
             "##.......[].##\n"
             "##........[]##\n"
             "##############\n";

  output.str("");
  output << map;

  EXPECT_EQ(output.str(), expected);
}
