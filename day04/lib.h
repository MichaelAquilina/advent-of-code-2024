#include <filesystem>
#include <vector>

std::vector<std::vector<char>> read_data(const std::filesystem::path &path);

uint get_part1(const std::vector<std::vector<char>> &data);

uint get_part2(const std::vector<std::vector<char>> &data);
