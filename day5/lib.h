#include <filesystem>
#include <map>
#include <optional>
#include <tuple>
#include <vector>

std::tuple<std::map<int, std::vector<int>>, std::vector<std::vector<int>>>
read_data(const std::filesystem::path &path);

std::tuple<uint, uint> get_parts(std::map<int, std::vector<int>> &queue,
                                 const std::vector<std::vector<int>> &jobs);
