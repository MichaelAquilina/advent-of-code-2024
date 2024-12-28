#include "lib.h"
#include "absl/strings/str_split.h"
#include "absl/strings/strip.h"
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

Point get_point(absl::string_view buffer) {
  std::vector<std::string> tokens = absl::StrSplit(buffer, ", ");
  std::string x_token(tokens.at(0).substr(2));
  std::string y_token(tokens.at(1).substr(2));

  auto x = std::stoi(x_token);
  auto y = std::stoi(y_token);

  return {x, y};
}

std::vector<GameConfig> read_data(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Unable to open file");
  }

  std::vector<GameConfig> output;
  std::stringstream buffer;

  buffer << file.rdbuf();
  const auto &data = buffer.str();

  for (auto &chunk : absl::StrSplit(data, "\n\n")) {
    std::vector<std::string> lines = absl::StrSplit(chunk, "\n");
    if (lines.size() < 3) {
      throw std::runtime_error("Invalid game config format");
    }
    auto button_a = get_point(absl::StripPrefix(lines.at(0), "Button A: "));
    auto button_b = get_point(absl::StripPrefix(lines.at(1), "Button B: "));
    auto price = get_point(absl::StripPrefix(lines.at(2), "Prize: "));

    output.push_back({button_a, button_b, price});
  }

  return output;
}

unsigned long solve(const GameConfig &config, unsigned long add,
                    bool debug = false) {
  const uint cost_a = 3;
  const uint cost_b = 1;
  // using linear algebra we can figure out
  // Px = Aax + Bbx
  // Py = Aay + Bby
  //
  // We can derive: A = (Pybx - Pxby) / (aybx - axby)
  //                B = (Px - Aax) / bx

  const auto bx = config.button_b.x;
  const auto by = config.button_b.y;
  const auto px = config.prize.x + add;
  const auto py = config.prize.y + add;
  const auto ax = config.button_a.x;
  const auto ay = config.button_a.y;

  long numerator = (py * bx) - (px * by);
  long denominator = (ay * bx) - (ax * by);

  const auto A = numerator / denominator;
  if (debug) {
    std::cout << " ax=" << ax << " ay=" << ay << "\n"
              << " bx=" << bx << " by=" << by << "\n"
              << " px=" << px << " py=" << py << "\n"
              << " py*bx=" << px * bx << " px*by=" << px * by << "\n";

    std::cout << numerator << " / " << denominator << " = " << A << "\n";
  }

  // A can only be whole numbers
  if (numerator % denominator != 0) {
    return 0;
  }

  numerator = px - (ax * A);

  // Similarly, A can only be whole numbers
  if (numerator % bx != 0) {
    return 0;
  }
  const long B = numerator / bx;

  return (A * cost_a) + (B * cost_b);
}

unsigned long get_part1(const std::vector<GameConfig> &configs) {
  unsigned long total = 0;
  for (auto &config : configs) {
    auto cost = solve(config, 0);
    total += cost;
  }
  return total;
}

unsigned long get_part2(const std::vector<GameConfig> &configs) {
  unsigned long total = 0;
  const unsigned long add = 10000000000000;
  for (auto &config : configs) {
    total += solve(config, add);
  }
  return total;
}
