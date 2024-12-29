#include "common/point.h"
#include <ostream>
#include <vector>

enum class MapObject {
  Wall,
  Empty,
};

struct MapPath;

struct Map {
  Point start;
  Point end;
  std::vector<std::vector<MapObject>> points;

  MapObject get_point(const Point &point) const;
  MapPath get_path() const;
};

struct MapPath {
  const Map &map;
  std::vector<Point> points;
  uint score;
};

std::ostream &operator<<(std::ostream &os, const MapPath &map_path);

uint get_cost(const Direction &dir1, const Direction &dir2);

uint get_part1(const Map &map);

Map read_data(std::istream &stream);
