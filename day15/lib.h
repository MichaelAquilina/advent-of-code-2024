#include "common/point.h"
#include <istream>
#include <vector>

enum class MapObject {
  Obstacle,
  ObstacleStart,
  ObstacleEnd,
  Wall,
  Empty,
};

std::ostream &operator<<(std::ostream &os, const MapObject &object);

struct Map {
  std::vector<std::vector<MapObject>> objects;
  Point robot;

  MapObject get_point(const Point &point) const;
  void set_point(const Point &point, const MapObject &object);

  void move(const Direction &direction);
};
std::ostream &operator<<(std::ostream &os, const Map &map);

uint get_part(Map &map, const std::vector<Direction> &directions);

Map scale_up(const Map &map);

std::tuple<Map, std::vector<Direction>> read_data(std::istream &stream);
