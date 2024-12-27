#include "point.h"
#include <filesystem>
#include <vector>

enum class MapObject {
  Obstacle,
  Wall,
  Empty,
};

struct Map {
  std::vector<std::vector<MapObject>> objects;
  Point robot;

  MapObject get_point(const Point &point) const;
  void set_point(const Point &point, const MapObject &object);

  void move(const Direction &direction);
};
std::ostream &operator<<(std::ostream &os, const Map &map);

uint get_part1(Map &map, const std::vector<Direction> &directions);

std::tuple<Map, std::vector<Direction>>
read_data(const std::filesystem::path &path);
