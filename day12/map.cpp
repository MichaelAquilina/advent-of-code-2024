#include "map.h"

Map::Map(std::vector<std::vector<char>> points, uint width, uint height)
    : points_(points), width_(width), height_(height) {}

std::tuple<uint, uint> Map::get_dimensions() const { return {width_, height_}; }

char Map::get_point(const Point &point) const {
  return points_.at(point.y).at(point.x);
}

bool Map::is_out_of_bounds(const Point &point) const {
  return (point.x < 0 || point.y < 0 || point.x >= (int)width_ ||
          point.y >= (int)height_);
}

Fence get_fence(const Map &map, const Point &point, std::set<Point> &visited) {
  const auto value = map.get_point(point);
  std::vector<Point> stack = {point};
  std::set<Point> points = {point};

  while (!stack.empty()) {
    Point point = stack.back();
    stack.pop_back();
    // could probably just use visited for better performance
    visited.insert(point);

    for (auto [_, neighbor] : point.get_neighbors()) {
      if (!map.is_out_of_bounds(neighbor) && map.get_point(neighbor) == value &&
          !visited.contains(neighbor) && !points.contains(neighbor)) {
        stack.push_back(neighbor);
        points.insert(neighbor);
      }
    }
  }

  return Fence{value, points};
}

std::vector<Fence> Map::get_fences() const {
  std::set<Point> visited;
  std::vector<Fence> output;

  for (uint y = 0; y < height_; y++) {
    for (uint x = 0; x < width_; x++) {
      Point point = {(int)x, (int)y};
      if (!visited.contains(point)) {
        output.push_back(get_fence(*this, point, visited));
      }
    }
  }

  return output;
}

void flood_fill(const Point &point, const Direction &direction,
                const Fence &fence,
                std::set<std::tuple<Direction, Point>> &visited) {

  std::vector<Point> stack = {point};
  visited.insert({direction, point});

  while (!stack.empty()) {
    Point current = stack.back();
    stack.pop_back();

    for (auto &[_, neighbor] : current.get_perpendicular_neighbors(direction)) {
      Point inverse_point = neighbor + inverse(direction);
      if (!visited.contains({direction, neighbor}) &&
          !fence.contains(neighbor) && fence.contains(inverse_point)) {
        stack.push_back(neighbor);
        visited.insert({direction, neighbor});
      }
    }
  }
}

uint Fence::get_sides() const {
  uint total = 0;
  std::set<std::tuple<Direction, Point>> visited;
  for (auto &point : points) {
    for (auto &[direction, neighbor] : point.get_neighbors()) {
      if (!visited.contains({direction, neighbor}) && !contains(neighbor)) {

        total++;
        flood_fill(neighbor, direction, *this, visited);
      }
    }
  }
  return total;
}

uint Fence::get_area() const { return points.size(); }

uint Fence::get_perimeter_length() const {
  uint total = 0;
  for (auto &point : points) {
    for (auto [_, neighbor] : point.get_neighbors()) {
      if (!points.contains(neighbor)) {
        total++;
      }
    }
  }
  return total;
}

bool Fence::contains(const Point &point) const {
  return points.find(point) != points.end();
}
