#include <filesystem>
#include <sys/types.h>
#include <vector>

struct Equation {
  unsigned long total;
  std::vector<unsigned long> operands;
};

enum class Operator {
  Add,
  Multiply,
  Concat,
};

std::vector<Equation> read_data(const std::filesystem::path &path);

std::vector<std::vector<Operator>>
generate_permutations(uint size, std::vector<Operator> &options);

unsigned long calculate(const std::vector<unsigned long> &operands,
                        const std::vector<Operator> &operators);

unsigned long check_matches(const std::vector<Equation> &equations,
                            std::vector<Operator> &operators);
