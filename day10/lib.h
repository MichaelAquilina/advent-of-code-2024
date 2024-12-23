#include <cstddef>
#include <filesystem>
#include <optional>
#include <sys/types.h>
#include <vector>

struct Point {
  int x;
  int y;

  bool operator<(const Point &other) const;
};

class Map {
public:
  Map(std::vector<std::vector<std::optional<uint>>> points, uint width,
      uint height, std::vector<Point> starting_points);

  std::tuple<uint, uint> get_dimensions() const;

  bool is_out_of_bounds(const Point &point) const;

  uint traverse(bool rating = false) const;

  std::optional<uint> get_value(const Point &point) const;

private:
  const std::vector<std::vector<std::optional<uint>>> points_;
  const std::vector<Point> starting_points_;
  const uint width_;
  const uint height_;
};

Map read_data(const std::filesystem::path &path);
