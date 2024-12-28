#include "common/point.h"
#include <filesystem>

struct GameConfig {
  Point button_a;
  Point button_b;
  Point prize;
};

std::vector<GameConfig> read_data(const std::filesystem::path &path);

unsigned long get_part1(const std::vector<GameConfig> &configs);

unsigned long get_part2(const std::vector<GameConfig> &configs);
