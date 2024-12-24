#include "point.h"
#include <iostream>
#include <vector>

Point Point::operator+(const Direction &direction) const {
  switch (direction) {
  case Direction::Up:
    return Point{x, y - 1};
  case Direction::Down:
    return Point{x, y + 1};
  case Direction::Right:
    return Point{x + 1, y};
  case Direction::Left:
    return Point{x - 1, y};
  }
  throw std::runtime_error("Unknown direction");
}

bool Point::operator<(const Point &other) const {
  if (y == other.y) {
    return x < other.x;
  } else {
    return y < other.y;
  }
}

bool Point::operator==(const Point &other) const {
  return x == other.x && y == other.y;
}

std::ostream &operator<<(std::ostream &os, const Point &point) {
  return os << "(" << point.x << "," << point.y << ")";
}

std::ostream &operator<<(std::ostream &os, const Direction &direction) {
  std::string value;
  switch (direction) {
  case Direction::Up:
    value = "Up";
    break;
  case Direction::Down:
    value = "Down";
    break;
  case Direction::Left:
    value = "Left";
    break;
  case Direction::Right:
    value = "Right";
    break;
  }
  return os << value;
}

Direction inverse(const Direction &direction) {
  switch (direction) {
  case Direction::Up:
    return Direction::Down;
  case Direction::Down:
    return Direction::Up;
  case Direction::Right:
    return Direction::Left;
  case Direction::Left:
    return Direction::Right;
  }
  throw std::runtime_error("Unknown direction");
}

std::vector<std::tuple<Direction, Point>> Point::get_neighbors() const {
  return {
      {Direction::Right, Point{x + 1, y}},
      {Direction::Left, Point{x - 1, y}},
      {Direction::Up, Point{x, y - 1}},
      {Direction::Down, Point{x, y + 1}},
  };
}

std::vector<std::tuple<Direction, Point>>
Point::get_perpendicular_neighbors(const Direction &direction) const {
  if (direction == Direction::Up || direction == Direction::Down) {
    return {
        {Direction::Right, Point{x + 1, y}},
        {Direction::Left, Point{x - 1, y}},
    };
  } else {
    return {
        {Direction::Up, Point{x, y - 1}},
        {Direction::Down, Point{x, y + 1}},
    };
  }
}
