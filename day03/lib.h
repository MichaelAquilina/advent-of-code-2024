#include <filesystem>
#include <vector>

enum class InstructionType { Mul, Do, Dont };

struct instruction {
  InstructionType instruction_type;
  int arg1;
  int arg2;
};

std::vector<instruction> read_data(const std::filesystem::path &path);

uint calculate(const std::vector<instruction> &data, bool dodont = false);
