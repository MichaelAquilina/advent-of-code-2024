#include "lib.h"
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <stdexcept>

uint get_cost(const Direction &dir1, const Direction &dir2) {
  const int d1 = get_degrees(dir1);
  const int d2 = get_degrees(dir2);

  return 1000 * (std::abs(d1 - d2) / 90);
}

struct Path {
  std::vector<Point> points;
  Direction direction;
  uint score;
};

std::ostream &operator<<(std::ostream &os, const MapPath &path) {
  for (uint y = 0; y < path.map.points.size(); y++) {
    for (uint x = 0; x < path.map.points.at(y).size(); x++) {
      auto point = Point((int)x, (int)y);
      auto item = path.map.get_point(point);
      if (point == path.map.start) {
        os << "S";
      } else if (point == path.map.end) {
        os << "E";
      } else if (std::find(path.points.begin(), path.points.end(), point) !=
                 path.points.end()) {
        os << "x";
      } else if (item == MapObject::Wall) {
        os << "#";
      } else if (item == MapObject::Empty) {
        os << ".";
      }
    }
    os << "\n";
  }
  return os;
}

// Depth First Search
MapPath Map::get_path() const {
  Path best_path = Path({}, Direction::Right, UINT_MAX);

  std::vector<Path> stack = {Path({start}, Direction::Right, 0)};

  while (!stack.empty()) {
    auto path = stack.back();
    stack.pop_back();

    auto current = path.points.back();
    auto direction = path.direction;

    if (current == end && path.score <= best_path.score) {
      best_path = path;
    }

    for (auto neighbor : current.get_neighbors()) {
      auto &[dir, point] = neighbor;
      auto item = get_point(point);
      if (item == MapObject::Empty &&
          std::find(path.points.begin(), path.points.end(), point) ==
              path.points.end()) {
        auto cost = get_cost(direction, dir);

        std::vector<Point> points(path.points.begin(), path.points.end());
        points.push_back(point);

        Path new_path = Path(points, dir, cost + path.score + 1);
        stack.push_back(new_path);
      }
    }
  }

  return MapPath{*this, best_path.points, best_path.score};
}

MapObject Map::get_point(const Point &point) const {
  return points.at(point.y).at(point.x);
}

uint get_part1(const Map &map) {
  auto map_path = map.get_path();
  return map_path.score;
}

Map read_data(std::istream &stream) {
  std::string line;
  std::vector<std::vector<MapObject>> objects;
  Point start;
  Point end;

  while (std::getline(stream, line)) {
    std::vector<MapObject> row;
    for (uint x = 0; x < line.size(); x++) {
      auto &item = line.at(x);

      if (item == '#') {
        row.push_back(MapObject::Wall);
      } else if (item == '.') {
        row.push_back(MapObject::Empty);
      } else if (item == 'S') {
        start = Point((int)x, (int)objects.size());
        row.push_back(MapObject::Empty);
      } else if (item == 'E') {
        end = Point((int)x, (int)objects.size());
        row.push_back(MapObject::Empty);
      } else {
        throw std::runtime_error("Unknown map object: " + std::to_string(item));
      }
    }
    objects.push_back(row);
  }

  return Map{start, end, objects};
}
