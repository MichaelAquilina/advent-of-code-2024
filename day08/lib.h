#include "common/point.h"
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <tuple>

class Map {
public:
  Map(std::map<char, std::set<Point>> antennas, uint width, uint height);

  std::tuple<uint, uint> get_dimensions() const;

  uint get_antenna_count() const;

  bool is_out_of_bounds(Point &point) const;

  std::optional<char> get_antenna(Point &point) const;

  const std::map<char, std::set<Point>> &get_antennas() const;

private:
  const std::map<char, std::set<Point>> antennas_;
  const uint width_;
  const uint height_;
};

Map read_data(const std::filesystem::path &path);

void print_map(Map &map, std::set<Point> &antinodes);

std::set<Point> get_antinodes(Map &map, bool resonant = false);
