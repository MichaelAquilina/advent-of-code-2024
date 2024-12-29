#include "point.h"
#include <iostream>
#include <sstream>
#include <string>

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

Point Point::operator*(int value) const { return {x * value, y * value}; }

Point Point::operator*(const Point &point) const {
  return {x * point.x, y * point.y};
}

Point Point::operator+(const Point &point) const {
  return {x + point.x, y + point.y};
}

Point Point::operator%(const Point &other) const {
  return {
      ((x % other.x) + other.x) % other.x,
      ((y % other.y) + other.y) % other.y,
  };
}

bool Point::operator<(const Point &other) const {
  if (y == other.y) {
    return x < other.x;
  } else {
    return y < other.y;
  }
}

Point Point::operator-(const Point &other) const {
  return Point{x - other.x, y - other.y};
}

Point Point::operator-() const { return Point{-x, -y}; }

bool Point::operator==(const Point &other) const {
  return x == other.x && y == other.y;
}

bool Point::operator!=(const Point &other) const { return !(*this == other); }

uint distance(const Point &point1, const Point &point2) {
  return std::abs(point1.x - point2.x) + std::abs(point1.y - point2.y);
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

Direction rotate_direction(const Direction &direction) {
  switch (direction) {
  case Direction::Up:
    return Direction::Right;
  case Direction::Right:
    return Direction::Down;
  case Direction::Down:
    return Direction::Left;
  case Direction::Left:
    return Direction::Up;
  }
  throw std::runtime_error("Unknown Direction");
}

Direction get_direction(const Point &point1, const Point &point2) {
  const Point diff = point1 - point2;
  if (diff == Point(0, -1)) {
    return Direction::Up;
  } else if (diff == Point(0, 1)) {
    return Direction::Down;
  } else if (diff == Point(-1, 0)) {
    return Direction::Left;
  } else if (diff == Point(1, 0)) {
    return Direction::Right;
  }
  std::ostringstream stream;
  stream << diff;
  throw std::runtime_error("Invalid Direction: " + stream.str());
}

uint get_degrees(const Direction &direction) {
  switch (direction) {
  case Direction::Up:
    return 0;
  case Direction::Right:
    return 90;
  case Direction::Down:
    return 180;
  case Direction::Left:
    return 270;
  }
  throw std::runtime_error("Unknown direction");
}

std::set<std::tuple<Direction, Point>> Point::get_neighbors() const {
  return {
      {Direction::Right, Point{x + 1, y}},
      {Direction::Left, Point{x - 1, y}},
      {Direction::Up, Point{x, y - 1}},
      {Direction::Down, Point{x, y + 1}},
  };
}

std::set<std::tuple<Direction, Point>>
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
