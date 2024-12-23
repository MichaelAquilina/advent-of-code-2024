#include <optional>
#include <vector>
#include <filesystem>
#include <sys/types.h>

struct Block {
    std::optional<uint> file_id;

    bool is_free() const;
};

void print_blocks(const std::vector<Block> &blocks);

std::vector<Block> read_data(const std::filesystem::path &path);

void compact_blocks(std::vector<Block> &blocks);

void compact_blocks_v2(std::vector<Block> &blocks);

unsigned long compute_checksum(const std::vector<Block> &blocks);

unsigned long get_part1(const std::vector<Block> &blocks);

unsigned long get_part2(const std::vector<Block> &blocks);
