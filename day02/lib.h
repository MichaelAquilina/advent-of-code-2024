#include <filesystem>
#include <vector>

std::vector<std::vector<uint>> read_data(const std::filesystem::path &path);

uint get_part1(const std::vector<std::vector<uint>> &reports);

uint get_part2(const std::vector<std::vector<uint>> &reports);

bool is_safe(std::vector<uint> report, int tolerate = 0);
