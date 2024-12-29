#include "lib.h"
#include <climits>
#include <cstdlib>
#include <map>
#include <optional>
#include <stdexcept>
#include <thread>

uint get_cost(const Direction &dir1, const Direction &dir2) {
  const int d1 = get_degrees(dir1);
  const int d2 = get_degrees(dir2);

  return 1000 * (std::abs(d1 - d2) / 90);
}

bool contains(const std::vector<std::tuple<Direction, Point>> &points,
              const Point &point) {
  for (auto &[_, item] : points) {
    if (item == point) {
      return true;
    }
  }
  return false;
}

std::ostream &operator<<(std::ostream &os, const MapPath &path) {
  for (uint y = 0; y < path.map.points.size(); y++) {
    for (uint x = 0; x < path.map.points.at(y).size(); x++) {
      auto point = Point((int)x, (int)y);
      auto item = path.map.get_point(point);
      if (point == path.map.start) {
        os << "S";
      } else if (point == path.map.end) {
        os << "E";
      } else if (contains(path.points, point)) {
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

void clear() { std::cout << "\033[2J\033[H"; }

std::vector<std::tuple<Direction, Point>>
reconstruct_path(const Point &point, const Direction &direction,
                 const std::map<Point, Point> came_from) {
  std::vector<std::tuple<Direction, Point>> path = {{direction, point}};
  Point current = point;

  while (came_from.contains(current)) {
    Point next = came_from.at(current);
    Direction dir = get_direction(next, current);
    current = next;
    path.push_back({dir, current});
  }
  return path;
}

unsigned long heuristic(const Point &point1, const Point &point2) {
  unsigned long distance_x = std::abs(point1.x - point2.x);
  unsigned long distance_y = std::abs(point1.y - point2.y);
  return distance_x + distance_y;
}

// A Star Algorithm
std::optional<MapPath> Map::get_path() const {
  std::map<Point, unsigned long> fscores = {{{start}, heuristic(start, end)}};
  std::map<Point, unsigned long> gscores = {{{start}, 0}};
  std::set<std::tuple<Direction, Point>> stack = {{Direction::Right, start}};
  std::map<Point, Point> came_from;

  while (!stack.empty()) {
    // choose cheapest from the stack
    uint min_score = UINT_MAX;
    std::tuple<Direction, Point> min_item;

    for (auto &item : stack) {
      auto it = fscores.find(std::get<1>(item));
      if (it != fscores.end() && it->second < min_score) {
        min_score = it->second;
        min_item = item;
      }
    }

    stack.erase(min_item);
    const Direction direction = std::get<0>(min_item);
    const Point current = std::get<1>(min_item);

    if (current == end) {
      return MapPath(*this, reconstruct_path(current, direction, came_from),
                     gscores.at(current));
    }

    for (auto &[dir, point] : current.get_neighbors()) {
      auto object = get_point(point);
      if (object == MapObject::Wall) {
        continue;
      }
      auto cost = get_cost(dir, direction);
      auto score = gscores.at(current) + cost + 1;
      if (!gscores.contains(point) || score < gscores.at(point)) {
        came_from[point] = current;

        gscores[point] = score;
        fscores[point] = score + heuristic(start, end);
        stack.insert({dir, point});
      }
    }
  }
  return std::nullopt;
}

MapObject Map::get_point(const Point &point) const {
  return points.at(point.y).at(point.x);
}

uint get_part1(const Map &map) {
  auto map_path = map.get_path();

  if (map_path.has_value()) {
    return map_path->score;
  } else {
    return UINT_MAX;
  }
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
