#include "lib.h"
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>

bool Block::is_free() const { return !file_id.has_value(); }

void print_blocks(const std::vector<Block> &blocks) {
  for (auto &block : blocks) {
    if (block.is_free()) {
      std::cout << ".";
    } else if (*block.file_id >= 10) {
      std::cout << '[' << *block.file_id << ']';
    } else {
      std::cout << *block.file_id;
    }
  }
  std::cout << std::endl;
}

std::vector<Block> read_data(const std::filesystem::path &path) {
  std::ifstream input_file(path);

  if (!input_file) {
    throw std::runtime_error("Unable to read file");
  }

  uint i = 0;
  std::string line;
  std::vector<Block> output;
  // TODO: should we be discarding blocks with size 0?
  while (std::getline(input_file, line)) {
    for (auto &item : line) {
      uint size = item - '0';
      std::optional<uint> file_id = std::nullopt;
      if (i % 2 == 0) {
        file_id = i / 2;
      }
      for (uint j = 0; j < size; j++) {
        output.push_back(Block{file_id});
      }
      i++;
    }
  }
  return output;
}

bool is_compacted(std::vector<Block> &blocks) {
  bool check = false;
  for (auto &blk : blocks) {
    if (blk.is_free()) {
      check = true;
    } else if (check) {
      return false;
    }
  }
  return true;
}

unsigned long get_part1(const std::vector<Block> &blocks) {
  std::vector<Block> copied_blocks(blocks);
  compact_blocks(copied_blocks);
  return compute_checksum(copied_blocks);
}

unsigned long get_part2(const std::vector<Block> &blocks) {
  std::vector<Block> copied_blocks(blocks);
  compact_blocks_v2(copied_blocks);
  return compute_checksum(copied_blocks);
}

void compact_blocks(std::vector<Block> &blocks) {
  uint position = 0;
  for (uint i = blocks.size() - 1; i > 0; i--) {
    if (is_compacted(blocks)) {
      return;
    }

    if (blocks.at(i).is_free()) {
      continue;
    }
    while (!blocks.at(position).is_free()) {
      position++;
    }

    blocks.at(position).file_id = blocks.at(i).file_id;
    blocks.at(i).file_id = std::nullopt;
    position = 0;
  }
}

// TODO: this could easily be generated at read_data instead!
std::map<uint, std::tuple<uint, uint>>
scan_file_map(const std::vector<Block> &blocks) {
  std::map<uint, std::tuple<uint, uint>> file_map;
  std::optional<uint> current_id = std::nullopt;
  uint size = 1;
  uint i;
  for (i = 0; i < blocks.size(); i++) {
    auto blk = blocks.at(i);

    if (current_id != blk.file_id) {
      if (current_id.has_value()) {
        file_map[*current_id] = {i - size, size};
      }
      current_id = blk.file_id;
      size = 1;
    } else {
      size++;
    }
  }
  if (current_id.has_value()) {
    file_map[*current_id] = {i - size, size};
  }
  return file_map;
}

bool has_free_space(const std::vector<Block> &blocks, uint position,
                    uint size) {
  if (position + size >= blocks.size()) {
    return false;
  }

  for (uint i = position; i < position + size; i++) {
    if (!blocks.at(i).is_free()) {
      return false;
    }
  }
  return true;
}

void compact_blocks_v2(std::vector<Block> &blocks) {
  std::map<uint, std::tuple<uint, uint>> file_map = scan_file_map(blocks);

  for (auto it = file_map.rbegin(); it != file_map.rend(); it++) {
    auto &[file_id, data] = *it;
    auto &[position, size] = data;

    for (uint freeptr = 0; freeptr < blocks.size(); freeptr++) {
      if (freeptr > position) {
        break;
      }

      if (has_free_space(blocks, freeptr, size)) {
        for (uint i = 0; i < size; i++) {
          blocks.at(freeptr + i).file_id = file_id;
          blocks.at(position + i).file_id = std::nullopt;
        }
        break;
      }
    }
  }
}

unsigned long compute_checksum(const std::vector<Block> &blocks) {
  unsigned long sum = 0;
  for (uint i = 0; i < blocks.size(); i++) {
    auto blk = blocks.at(i);
    if (!blk.is_free()) {
      sum += *blk.file_id * i;
    }
  }
  return sum;
}
