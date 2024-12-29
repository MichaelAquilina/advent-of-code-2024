#include <iostream>
#include <set>
#include <tuple>

enum class Direction {
  Up,
  Down,
  Left,
  Right,
};

Direction inverse(const Direction &direction);
Direction rotate_direction(const Direction &direction);
uint get_degrees(const Direction &direction);

std::ostream &operator<<(std::ostream &os, const Direction &direction);

struct Point {
  int x;
  int y;

  std::set<std::tuple<Direction, Point>> get_neighbors() const;
  std::set<std::tuple<Direction, Point>>
  get_perpendicular_neighbors(const Direction &direction) const;

  bool operator<(const Point &other) const;
  bool operator==(const Point &other) const;
  bool operator!=(const Point &other) const;

  Point operator-() const;
  Point operator-(const Point &other) const;
  Point operator+(const Point &other) const;
  Point operator%(const Point &other) const;
  Point operator+(const Direction &direction) const;
  Point operator*(int value) const;
  Point operator*(const Point &other) const;
};

Direction get_direction(const Point &point1, const Point &point2);

uint distance(const Point &point1, const Point &point2);

std::ostream &operator<<(std::ostream &os, const Point &point);
