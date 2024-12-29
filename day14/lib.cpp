#include "lib.h"
#include "absl/strings/str_split.h"
#include <cstdlib>
#include <fstream>
#include <map>
#include <set>
#include <stdexcept>

std::ostream &operator<<(std::ostream &os, const Robot &robot) {
  return os << "p=" << robot.position << " v=" << robot.velocity;
}

Point get_point(absl::string_view buffer) {
  const std::vector<absl::string_view> tokens =
      absl::StrSplit(buffer.substr(2), ",");

  std::string x(tokens.at(0));
  std::string y(tokens.at(1));
  return Point{stoi(x), stoi(y)};
}

std::vector<Robot> read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Unable to open file");
  }

  std::string line;
  std::vector<Robot> robots;
  while (std::getline(file, line)) {
    std::vector<absl::string_view> tokens = absl::StrSplit(line, " ");
    const auto start = get_point(tokens.at(0));
    const auto velocity = get_point(tokens.at(1));
    robots.push_back(Robot{start, velocity});
  }

  return robots;
}

void simulate(std::vector<Robot> &robots, const uint width, const uint height) {
  const Point dimensions = {(int)width, (int)height};
  for (auto &robot : robots) {
    robot.position = robot.position + robot.velocity;
    robot.position = robot.position % dimensions;
  }
}

void print_robots(std::ostream &os, const std::vector<Robot> &robots,
                  const uint width, const uint height, bool quadrants = false) {
  std::map<Point, uint> locations;
  for (auto &robot : robots) {
    locations[robot.position]++;
  }

  const int mid_width = width / 2;
  const int mid_height = height / 2;

  for (uint y = 0; y < height; y++) {
    for (uint x = 0; x < width; x++) {
      Point current = {(int)x, (int)y};
      if (quadrants && (current.x == mid_width || current.y == mid_height)) {
        os << " ";
      } else if (locations.contains(current)) {
        os << locations[current];
      } else {
        os << ".";
      }
    }
    os << "\n";
  }
  os << "\n";
}

std::tuple<uint, uint, uint, uint>
get_quadrants(const std::vector<Robot> &robots, uint width, uint height) {
  uint q1 = 0;
  uint q2 = 0;
  uint q3 = 0;
  uint q4 = 0;

  const int mid_width = width / 2;
  const int mid_height = height / 2;

  for (auto &robot : robots) {
    auto &position = robot.position;
    if (position.x < mid_width) {
      if (position.y < mid_height) {
        q1++;
      } else if (position.y > mid_height) {
        q2++;
      }
    } else if (position.x > mid_width) {
      if (position.y < mid_height) {
        q3++;
      } else if (position.y > mid_height) {
        q4++;
      }
    }
  }
  return {q1, q2, q3, q4};
}

uint get_safety_score(const std::vector<Robot> &robots, uint width, uint height,
                      bool debug) {
  auto [q1, q2, q3, q4] = get_quadrants(robots, width, height);

  if (debug) {
    print_robots(std::cout, robots, width, height);
    std::cout << q1 << ":" << q2 << ":" << q3 << ":" << q4 << "\n";
  }

  return q1 * q2 * q3 * q4;
}

unsigned long distance(const std::vector<Robot> &robots) {
  unsigned long total = 0;
  for (uint i = 0; i < robots.size(); i++) {
    for (uint j = 0; j < robots.size(); j++) {
      total += distance(robots.at(i).position, robots.at(j).position);
    }
  }
  return total;
}

uint get_part1(const std::vector<Robot> &robots, uint width, uint height) {
  const uint seconds = 100;
  std::vector<Robot> copied_robots(robots);

  for (uint i = 0; i < seconds; i++) {
    simulate(copied_robots, width, height);
  }
  return get_safety_score(copied_robots, width, height, false);
}

uint get_part2(const std::vector<Robot> &robots, uint width, uint height) {
  std::vector<Robot> copied_robots(robots);

  std::set<std::string> seen;

  unsigned long long elapsed = 0;
  while (true) {
    elapsed++;
    simulate(copied_robots, width, height);

    std::stringstream value;
    print_robots(value, copied_robots, width, height);

    auto d = distance(copied_robots);

    if (d < 10000000) {
      std::cout << value.str();
      std::cout << "Distance: " << d << "\n";
      std::cout << "Elapsed: " << elapsed << "\n";
      break;
    }

    if (seen.contains(value.str())) {
      std::cout << "Could not find tree. Cycle at " << elapsed << "\n";
      break;
    } else {
      seen.insert(value.str());
    }
  }
  return elapsed;
}
