#include "common/point.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Direction, inverse) {
  EXPECT_EQ(inverse(Direction::Up), Direction::Down);
  EXPECT_EQ(inverse(Direction::Down), Direction::Up);
  EXPECT_EQ(inverse(Direction::Right), Direction::Left);
  EXPECT_EQ(inverse(Direction::Left), Direction::Right);
}

TEST(Direction, reverseable) {
  EXPECT_EQ(inverse(inverse(Direction::Right)), Direction::Right);
}

TEST(Direction, RotateDirection) {
  EXPECT_EQ(rotate_direction(Direction::Right), Direction::Down);
  EXPECT_EQ(rotate_direction(Direction::Down), Direction::Left);
}

TEST(Direction, ostream) {
  std::stringstream os;

  os << Direction::Up;
  EXPECT_EQ(os.str(), "Up");
}

TEST(Point, ostream) {
  std::stringstream os;

  os << Point{10, -20};
  EXPECT_EQ(os.str(), "(10,-20)");
}

TEST(Point, GetNeighbors) {
  const Point point = {4, 3};

  const std::set<std::tuple<Direction, Point>> expected = {
      {Direction::Up, {4, 2}},
      {Direction::Right, {5, 3}},
      {Direction::Down, {4, 4}},
      {Direction::Left, {3, 3}},
  };

  EXPECT_EQ(point.get_neighbors(), expected);
}

TEST(Point, GetPerpendicularNeighbors) {
  const Point point = {11, 2};
  const Direction direction = Direction::Right;

  const std::set<std::tuple<Direction, Point>> expected = {
      {Direction::Up, {11, 1}},
      {Direction::Down, {11, 3}},
  };
  EXPECT_EQ(point.get_perpendicular_neighbors(direction), expected);
}

TEST(Point, Negate) {
  const Point point = {1, -4};
  const Point expected = {-1, 4};
  EXPECT_EQ(-point, expected);
}

TEST(Point, LessThan) {
  const Point point1 = {1, 1};
  const Point point2 = {1, 2};
  const Point point3 = {2, 2};
  EXPECT_TRUE(point1 < point2);
  EXPECT_TRUE(point2 < point3);
}

TEST(Point, Inequality) {
  const Point point1 = {10, 10};
  const Point point2 = {20, 10};
  EXPECT_TRUE(point1 != point2);
}

TEST(Point, AddPoint) {
  const Point point1 = {10, 20};
  const Point point2 = {20, -13};
  const Point expected = {30, 7};
  EXPECT_EQ(point1 + point2, expected);
}

TEST(Point, SubtractPoint) {
  const Point point1 = {10, 20};
  const Point point2 = {20, -13};
  const Point expected = {-10, 33};
  EXPECT_EQ(point1 - point2, expected);
}

TEST(Point, ModuloPoint) {
  const Point point1 = {-10, 20};
  const Point point2 = {30, 15};
  const Point expected = {20, 5};
  EXPECT_EQ(point1 % point2, expected);
}

TEST(Point, AddDirection) {
  const Point point = {10, 20};
  const Direction direction = Direction::Up;
  const Point expected = {10, 19};
  EXPECT_EQ(point + direction, expected);
}
