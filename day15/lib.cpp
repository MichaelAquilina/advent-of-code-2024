#include "lib.h"
#include <stdexcept>

std::ostream &operator<<(std::ostream &os, const MapObject &object) {
  if (object == MapObject::ObstacleStart) {
    return os << "ObstacleStart";
  } else if (object == MapObject::ObstacleEnd) {
    return os << "ObstacleEnd";
  } else if (object == MapObject::Obstacle) {
    return os << "Obstacle";
  } else if (object == MapObject::Empty) {
    return os << "Empty";
  } else if (object == MapObject::Wall) {
    return os << "Wall";
  } else {
    throw std::runtime_error("Unknown MapObject");
  }
}

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
      } else if (object == MapObject::ObstacleStart) {
        os << "[";
      } else if (object == MapObject::ObstacleEnd) {
        os << "]";
      } else if (object == MapObject::Empty) {
        os << ".";
      } else {
        throw std::runtime_error("Unknown MapObject");
      }
    }
    os << "\n";
  }
  return os;
}

Map scale_up(const Map &map) {
  std::vector<std::vector<MapObject>> objects;
  for (uint y = 0; y < map.objects.size(); y++) {
    auto &row = map.objects.at(y);
    std::vector<MapObject> new_row;
    for (uint x = 0; x < row.size(); x++) {
      auto &item = row.at(x);
      if (item == MapObject::Wall) {
        new_row.push_back(MapObject::Wall);
        new_row.push_back(MapObject::Wall);
      } else if (item == MapObject::Obstacle) {
        new_row.push_back(MapObject::ObstacleStart);
        new_row.push_back(MapObject::ObstacleEnd);
      } else if (item == MapObject::Empty) {
        new_row.push_back(MapObject::Empty);
        new_row.push_back(MapObject::Empty);
      } else {
        throw std::runtime_error("Cannot scale up map");
      }
    }
    objects.push_back(new_row);
  }
  Point start = map.robot * Point{2, 1};

  return Map{objects, start};
}

void run(Map &map, const std::vector<Direction> &directions,
         bool debug = false) {
  if (debug) {
    std::cout << "Initial State:\n";
    std::cout << map << "\n";
  }

  for (auto &direction : directions) {
    map.move(direction);

    if (debug) {
      std::cout << "Move " << direction << ":\n";
      std::cout << map << "\n";
    }
  }
}

uint get_gps(const Map &map) {
  uint total = 0;
  for (uint y = 0; y < map.objects.size(); y++) {
    for (uint x = 0; x < map.objects.at(y).size(); x++) {
      auto object = map.objects.at(y).at(x);
      if (object == MapObject::Obstacle || object == MapObject::ObstacleStart) {
        total += x + (y * 100);
      }
    }
  }
  return total;
}

uint get_part(Map &map, const std::vector<Direction> &directions) {
  run(map, directions);
  return get_gps(map);
}

MapObject Map::get_point(const Point &point) const {
  return objects.at(point.y).at(point.x);
}

void Map::set_point(const Point &point, const MapObject &object) {
  objects.at(point.y).at(point.x) = object;
}

std::tuple<bool, std::set<std::tuple<Point, MapObject>>>
get_pushed_objects(const Map &map, const Point &point,
                   const Direction &direction) {
  std::set<std::tuple<Point, MapObject>> output;

  std::vector<Point> stack;
  auto obj = map.get_point(point);
  stack.push_back(point);
  if (obj == MapObject::ObstacleStart) {
    stack.push_back(point + Direction::Right);
  } else if (obj == MapObject::ObstacleEnd) {
    stack.push_back(point + Direction::Left);
  }

  bool wall_hit = false;

  while (!stack.empty()) {
    auto current = stack.back();
    stack.pop_back();

    auto object = map.get_point(current);
    if (output.contains({current, object}) || object == MapObject::Empty) {
      continue;
    }

    output.insert({current, object});

    if (object == MapObject::Wall) {
      wall_hit = true;
    } else {
      stack.push_back(current + direction);
      if (direction == Direction::Up || direction == Direction::Down) {
        if (object == MapObject::ObstacleStart) {
          stack.push_back(current + Direction::Right);
        } else if (object == MapObject::ObstacleEnd) {
          stack.push_back(current + Direction::Left);
        }
      }
    }
  }

  return {wall_hit, output};
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
  } else if (object == MapObject::ObstacleEnd ||
             object == MapObject::ObstacleStart) {
    auto [hit_wall, objs] = get_pushed_objects(*this, next, direction);
    if (hit_wall) {
      return;
    }
    for (auto &[loc, _] : objs) {
      set_point(loc, MapObject::Empty);
    }
    for (auto &[loc, obj] : objs) {
      set_point(loc + direction, obj);
    }
    robot = next;
  } else {
    std::cerr << object << "\n";
    throw std::runtime_error("Unknown MapObject encountered");
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
        robot = Point{(int)x, (int)objects.size()};
        row.push_back(MapObject::Empty);
      } else if (item == '[') {
        row.push_back(MapObject::ObstacleStart);
      } else if (item == ']') {
        row.push_back(MapObject::ObstacleEnd);
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

std::tuple<Map, std::vector<Direction>> read_data(std::istream &stream) {

  auto map = read_map(stream);
  auto directions = read_directions(stream);

  return {map, directions};
}
