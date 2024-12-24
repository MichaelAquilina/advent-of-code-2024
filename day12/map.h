#include "point.h"
#include <set>
#include <sys/types.h>
#include <vector>

struct Fence {
  char value;
  std::set<Point> points;

  uint get_perimeter_length() const;

  uint get_sides() const;

  uint get_area() const;

  bool contains(const Point &) const;
};

class Map {
public:
  Map(std::vector<std::vector<char>> points, uint width, uint height);

  std::tuple<uint, uint> get_dimensions() const;

  std::vector<Fence> get_fences() const;

  bool is_out_of_bounds(const Point &point) const;

  char get_point(const Point &point) const;

private:
  const std::vector<std::vector<char>> points_;
  const uint width_;
  const uint height_;
};
