#include "map.h"
#include <filesystem>
#include <set>
#include <sys/types.h>
#include <vector>

Map read_data(const std::filesystem::path &path);

std::tuple<uint, uint> get_parts(const std::vector<Fence> &fences,
                                 bool debug_part1, bool debug_part2);
