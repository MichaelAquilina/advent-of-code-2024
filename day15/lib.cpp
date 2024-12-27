#include "lib.h"
#include <fstream>
#include <stdexcept>

std::ostream &operator<<(std::ostream &os, const Map &map) {
  for (uint y = 0; y < map.objects.size(); y++) {
    for (uint x = 0; x < map.objects.at(y).size(); x++) {
      auto object = map.objects.at(y).at(x);
      Point current = Point{(int)x, (int)y};
      if (current == map.robot) {
        os << "@";
      } else if (object == MapObject::Obstacle) {
        os << "O";
      } else if (object == MapObject::Wall) {
        os << "#";
      } else if (object == MapObject::Empty) {
        os << ".";
      }
    }
    os << "\n";
  }
  return os << "\n";
}

void run(Map &map, const std::vector<Direction> &directions,
         bool debug = false) {
  if (debug) {
    std::cout << "Initial State:\n";
    std::cout << map;
  }

  for (auto &direction : directions) {
    map.move(direction);

    if (debug) {
      std::cout << "Move " << direction << ":\n";
      std::cout << map;
    }
  }
}

uint get_gps(const Map &map) {
  uint total = 0;
  for (uint y = 0; y < map.objects.size(); y++) {
    for (uint x = 0; x < map.objects.at(y).size(); x++) {
      auto object = map.objects.at(y).at(x);
      if (object == MapObject::Obstacle) {
        total += x + (y * 100);
      }
    }
  }
  return total;
}

uint get_part1(Map &map, const std::vector<Direction> &directions) {
  run(map, directions);
  return get_gps(map);
}

MapObject Map::get_point(const Point &point) const {
  return objects.at(point.y).at(point.x);
}

void Map::set_point(const Point &point, const MapObject &object) {
  objects.at(point.y).at(point.x) = object;
}

void Map::move(const Direction &direction) {
  auto next = robot + direction;
  auto object = get_point(next);

  if (object == MapObject::Wall) {
    return;
  } else if (object == MapObject::Empty) {
    robot = next;
  } else if (object == MapObject::Obstacle) {
    auto nextnext = next;
    while (get_point(nextnext) == MapObject::Obstacle) {
      nextnext = nextnext + direction;
    }
    if (get_point(nextnext) == MapObject::Empty) {
      robot = next;
      set_point(next, MapObject::Empty);
      set_point(nextnext, MapObject::Obstacle);
    }
  }
}

Map read_map(std::istream &is) {
  std::string line;
  std::vector<std::vector<MapObject>> objects;
  Point robot;

  while (std::getline(is, line)) {
    if (line == "") {
      break;
    }

    std::vector<MapObject> row;
    for (uint x = 0; x < line.size(); x++) {
      auto &item = line.at(x);
      if (item == '#') {
        row.push_back(MapObject::Wall);
      } else if (item == 'O') {
        row.push_back(MapObject::Obstacle);
      } else if (item == '@') {
        robot = Point{(int)x, (int)row.size()};
        row.push_back(MapObject::Empty);
      } else if (item == '.') {
        row.push_back(MapObject::Empty);
      } else {
        throw std::runtime_error("Unknown item in map");
      }
    }
    objects.push_back(row);
  }

  return Map{objects, robot};
}

std::vector<Direction> read_directions(std::istream &is) {
  std::vector<Direction> directions;
  std::string line;
  while (std::getline(is, line)) {
    for (auto &item : line) {
      Direction direction;
      if (item == '>') {
        direction = Direction::Right;
      } else if (item == '<') {
        direction = Direction::Left;
      } else if (item == '^') {
        direction = Direction::Up;
      } else if (item == 'v') {
        direction = Direction::Down;
      } else {
        std::cerr << item << "\n";
        throw std::runtime_error("Unknown direction");
      }
      directions.push_back(direction);
    }
  }
  return directions;
}

std::tuple<Map, std::vector<Direction>>
read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Unable to open file!");
  }
  auto map = read_map(file);
  auto directions = read_directions(file);

  return {map, directions};
}
