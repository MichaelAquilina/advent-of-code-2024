#include "lib.h"
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <set>
#include <stdexcept>

bool Point::operator<(const Point &other) const {
  if (y == other.y) {
    return x < other.x;
  } else {
    return y < other.y;
  }
}

std::ostream &operator<<(std::ostream &os, const Point &point) {
  return os << "(" << point.x << "," << point.y << ")";
}

Map::Map(std::vector<std::vector<std::optional<uint>>> points, uint width,
         uint height, std::vector<Point> starting_points)
    : points_(points), starting_points_(starting_points), width_(width),
      height_(height) {}

std::tuple<uint, uint> Map::get_dimensions() const { return {width_, height_}; }

std::optional<uint> Map::get_value(const Point &point) const {
  return points_.at(point.y).at(point.x);
}

bool Map::is_out_of_bounds(const Point &point) const {
  return (point.x < 0 || point.y < 0 || point.x >= (int)width_ ||
          point.y >= (int)height_);
}

void check_path(const Map &map, const Point &current,
                std::map<Point, uint> &reached) {
  const uint current_value = *map.get_value(current);

  const std::vector<Point> routes = {
      Point{current.x, current.y - 1},
      Point{current.x, current.y + 1},
      Point{current.x + 1, current.y},
      Point{current.x - 1, current.y},
  };
  for (const Point &point : routes) {
    if (map.is_out_of_bounds(point)) {
      continue;
    }

    const std::optional<uint> new_value = map.get_value(point);
    if (new_value.has_value()) {
      if (*new_value == current_value + 1) {
        if (*new_value == 9) {
          reached[point] += 1;
        } else {
          check_path(map, point, reached);
        }
      }
    }
  }
}

uint Map::traverse(bool rating) const {
  uint count = 0;
  for (const Point &point : starting_points_) {
    std::map<Point, uint> reached;
    check_path(*this, point, reached);
    if (rating) {
      for (const auto [_, value] : reached) {
        count += value;
      }
    } else {
      count += reached.size();
    }
  }

  return count;
}

Map read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Unable to open file");
  }

  std::vector<std::vector<std::optional<uint>>> points;
  std::vector<Point> starting_points;
  std::string line;
  uint height = 0;
  uint width = 0;

  while (std::getline(file, line)) {
    std::vector<std::optional<uint>> row;
    width = line.size();
    for (uint x = 0; x < line.size(); x++) {
      char item = line.at(x);

      std::optional<uint> value = std::nullopt;

      if (item != '.') {
        value = line.at(x) - '0';
        if (value == 0) {
          starting_points.push_back(Point{(int)x, (int)height});
        }
      }
      row.push_back(value);
    }
    points.push_back(row);
    height++;
  }

  return Map(points, width, height, starting_points);
}
