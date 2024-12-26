#include <iostream>
#include <tuple>
#include <vector>

enum class Direction {
  Up,
  Down,
  Left,
  Right,
};

Direction inverse(const Direction &direction);

std::ostream &operator<<(std::ostream &os, const Direction &direction);

struct Point {
  unsigned long x;
  unsigned long y;

  std::vector<std::tuple<Direction, Point>> get_neighbors() const;
  std::vector<std::tuple<Direction, Point>>
  get_perpendicular_neighbors(const Direction &direction) const;

  bool operator<(const Point &other) const;
  bool operator==(const Point &other) const;

  Point operator*(unsigned long value) const;
  Point operator+(unsigned long value) const;
  Point operator+(const Point &other) const;
  Point operator-(const Point &other) const;
  Point operator+(const Direction &direction) const;
};

std::ostream &operator<<(std::ostream &os, const Point &direction);
