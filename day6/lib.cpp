#include "lib.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <thread>

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

bool Point::operator!=(const Point &other) const {
  return x != other.x || y != other.y;
}

std::ostream &operator<<(std::ostream &os, const Point &point) {
  return os << "(" << point.x << "," << point.y << ")";
}

Direction rotate_direction(Direction &direction) {
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

std::ostream &operator<<(std::ostream &os, const Direction &direction) {
  switch (direction) {
  case Direction::Up:
    return os << "^";
  case Direction::Right:
    return os << ">";
  case Direction::Down:
    return os << "v";
  case Direction::Left:
    return os << "<";
  }
  throw std::runtime_error("Unknown Direction");
}

std::tuple<Map, Point, Direction> read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Unable to read file");
  }

  std::vector<std::vector<PointType>> points;
  std::string line;
  uint width = 0;
  uint height = 0;
  Point start = {0, 0};

  while (getline(file, line)) {
    std::vector<PointType> row;
    for (uint x = 0; x < line.size(); x++) {
      char point = line.at(x);
      width = line.length();

      if (point == '.') {
        row.push_back(PointType::Empty);
      } else if (point == '#') {
        row.push_back(PointType::Obstacle);
      } else if (point == '^') {
        row.push_back(PointType::Empty);
        start = {(int)x, (int)height};
      }
    }
    height++;
    points.push_back(row);
  }

  return {Map(width, height, points), start, Direction::Up};
}

Map::Map(uint width, uint height, std::vector<std::vector<PointType>> points)
    : width_(width), height_(height), points_(points),
      current_position_({0, 0}), current_direction_(Direction::Up) {}

bool Map::is_out_of_bounds() { return is_out_of_bounds(current_position_); }

bool Map::is_out_of_bounds(Point &point) {
  return (point.x < 0 || point.x >= (int)width_ || point.y < 0 ||
          point.y >= (int)height_);
}

void Map::set_current_position(Point point, Direction direction) {
  current_position_ = point;
  current_direction_ = direction;
}

void Map::print_point(Point &point) {
  if (point == current_position_) {
    std::cout << current_direction_;
  } else if (is_out_of_bounds(point)) {
    std::cout << "-";
  } else {
    switch (get_point(point)) {
    case PointType::Empty:
      std::cout << ".";
      break;
    case PointType::Obstacle:
      std::cout << "#";
      break;
    }
  }
}

void Map::print_local(int scope, std::optional<Point> special) {
  for (int j = -scope; j <= scope; j++) {
    for (int i = -scope; i <= scope; i++) {
      Point current = {current_position_.x + i, current_position_.y + j};
      if (special.has_value() && current == *special) {
        std::cout << "O";
      } else {
        print_point(current);
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

Point get_next_position(const Point &current, const Direction &direction) {
  Point movement = {0, 0};
  switch (direction) {
  case Direction::Up:
    movement = {0, -1};
    break;
  case Direction::Down:
    movement = {0, 1};
    break;
  case Direction::Left:
    movement = {-1, 0};
    break;
  case Direction::Right:
    movement = {1, 0};
    break;
  }

  Point next_position = {current.x + movement.x, current.y + movement.y};
  return next_position;
}

std::tuple<Point, Direction> Map::next() {
  if (!is_out_of_bounds()) {
    auto next_position =
        get_next_position(current_position_, current_direction_);

    if (is_out_of_bounds(next_position)) {
      current_position_ = next_position;
    } else if (get_point(next_position) == PointType::Obstacle) {
      current_direction_ = rotate_direction(current_direction_);
    } else {
      current_position_ = next_position;
    }
  }

  return get_current_position();
}

void Map::add_obstacle(const Point &point) {
  points_.at(point.y).at(point.x) = PointType::Obstacle;
}

void Map::remove_obstacle(const Point &point) {
  points_.at(point.y).at(point.x) = PointType::Empty;
}

const PointType &Map::get_point(Point &point) const {
  return points_.at(point.y).at(point.x);
}

std::tuple<Point, Direction> Map::get_current_position() {
  return {current_position_, current_direction_};
}

std::tuple<uint, uint> Map::get_dimensions() const { return {width_, height_}; }

void clear() { std::cout << "\033[2J\033[H"; }

void print_vector(std::set<Point> &points) {
  for (auto &item : points) {
    std::cout << item;
  }
  std::cout << std::endl;
}

void print_map_start_info(Map &map) {
  auto [width, height] = map.get_dimensions();
  auto [start, direction] = map.get_current_position();

  std::cout << "Starting simulation\n";
  std::cout << "Map size: " << width << "x" << height << std::endl;
  std::cout << "Start: " << start << " " << direction << std::endl;
}

std::tuple<std::map<Point, std::set<Direction>>, bool>
run_to_end(Map &map, Point start, Direction direction, bool debug) {
  map.set_current_position(start, direction);
  std::map<Point, std::set<Direction>> visited = {
      {start, {direction}},
  };

  if (debug) {
    print_map_start_info(map);
  }

  auto [width, _] = map.get_dimensions();
  int scope = std::min(20, (int)width);

  while (!map.is_out_of_bounds()) {
    auto [position, direction] = map.next();

    if (visited[position].contains(direction)) {
      // we're stuck in a loop!
      return {visited, false};
    } else {
      visited[position].insert(direction);
    }

    if (debug) {
      clear();
      std::cout << "Position: " << position << std::endl;
      std::cout << "Total: " << visited.size() << std::endl;
      std::cout << "Next: " << position << std::endl;
      map.print_local(scope);
      std::cout << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(70));
    }
  }

  return {visited, true};
}

std::set<Point> find_loop_points(Map &map,
                                 std::map<Point, std::set<Direction>> &visited,
                                 Point start, Direction start_direction,
                                 bool debug) {
  std::set<Point> loop_points;

  auto [width, height] = map.get_dimensions();

  // Tried the non-brute force approach but it was missing some
  // corner cases I could not figure out :(
  // finishes in under 20 seconds so its not too bad anyway
  for (uint i = 0; i < width; i++) {
    for (uint j = 0; j < height; j++) {
      Point test_point = {(int)i, (int)j};

      if (!loop_points.contains(test_point) && test_point != start &&
          !map.is_out_of_bounds(test_point) &&
          map.get_point(test_point) != PointType::Obstacle) {

        map.add_obstacle(test_point);

        const auto [_, finished] =
            run_to_end(map, start, start_direction, debug);

        if (debug && !finished) {
          std::cout << "found loop obstacle at " << test_point << std::endl;
          map.set_current_position(start, start_direction);
          map.print_local(10, test_point);
        }

        map.remove_obstacle(test_point);

        if (!finished) {
          loop_points.insert(test_point);
        }
      }
    }
  }

  return loop_points;
}
