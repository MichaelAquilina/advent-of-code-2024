#include "common/point.h"
#include <filesystem>
#include <vector>

struct Robot {
  Point position;
  Point velocity;
};

std::ostream &operator<<(std::ostream &os, const Robot &robot);

std::vector<Robot> read_data(const std::filesystem::path &path);

void simulate(std::vector<Robot> &robots, uint width, uint height);

uint get_part1(const std::vector<Robot> &robots, uint width, uint height);

uint get_part2(const std::vector<Robot> &robots, uint width, uint height);
