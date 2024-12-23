#include <filesystem>
#include <sys/types.h>
#include <vector>

std::vector<unsigned long> read_data(const std::filesystem::path &path);

std::vector<unsigned long> blink(std::vector<unsigned long> &stones,
                                 uint count);

unsigned long blink_many(std::vector<unsigned long> &stones, uint count = 25);
