#include "lib.h"
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

Map::Map(std::map<char, std::set<Point>> antennas, uint width, uint height)
    : antennas_(antennas), width_(width), height_(height) {}

bool Map::is_out_of_bounds(Point &point) const {
  return (point.x < 0 || point.y < 0 || point.x >= (int)width_ ||
          point.y >= (int)height_);
}

std::tuple<uint, uint> Map::get_dimensions() const { return {width_, height_}; }

uint Map::get_antenna_count() const {
  uint total = 0;
  for (auto &[_, points] : antennas_) {
    total += points.size();
  }
  return total;
}

const std::map<char, std::set<Point>> &Map::get_antennas() const {
  return antennas_;
}

std::optional<char> Map::get_antenna(Point &point) const {
  for (auto &[frequency, antennas] : antennas_) {
    if (antennas.contains(point)) {
      return frequency;
    }
  }
  return std::nullopt;
}

Map read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Unable to open file");
  }

  std::string line;
  std::map<char, std::set<Point>> antennas;
  uint height = 0;
  uint width = 0;

  while (std::getline(file, line)) {
    width = line.size();
    for (uint x = 0; x < line.size(); x++) {
      char item = line.at(x);
      if (item == '.') {
        continue;
      } else {
        Point point = {(int)x, (int)height};
        antennas[item].insert(point);
      }
    }
    height++;
  }

  return Map(antennas, width, height);
}

void print_map(Map &map, std::set<Point> &antinodes) {
  const auto &[width, height] = map.get_dimensions();
  for (uint j = 0; j < height; j++) {
    for (uint i = 0; i < width; i++) {
      Point current = {(int)i, (int)j};
      auto frequency = map.get_antenna(current);

      if (antinodes.contains(current)) {
        std::cout << "#";
      } else if (frequency.has_value()) {
        std::cout << *frequency;
      } else {
        std::cout << ".";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

std::vector<Point> produce_antinodes(Map &map, const Point &a, Point &vector,
                                     bool resonant) {
  std::vector<Point> output;
  bool out_of_bounds;
  Point current = a;
  do {
    current = current + vector;
    out_of_bounds = map.is_out_of_bounds(current);
    if (!out_of_bounds) {
      output.push_back(current);
    }

  } while (resonant && !out_of_bounds);
  return output;
}

std::set<Point> get_antinodes(Map &map, bool resonant) {
  std::set<Point> output;
  for (const auto &[frequency, antennas] : map.get_antennas()) {
    for (const auto &a1 : antennas) {
      if (resonant) {
        output.insert(a1);
      }
      for (const auto &a2 : antennas) {
        if (a1 == a2) {
          continue;
        }

        Point vector = a1 - a2;
        std::vector<Point> antinodes =
            produce_antinodes(map, a1, vector, resonant);
        for (Point &antinode : antinodes) {
          output.insert(antinode);
        }
      }
    }
  }
  return output;
}
