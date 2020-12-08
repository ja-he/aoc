#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include <cassert>

struct Instruction
{
  std::string opcode;
  int         operand;
};

std::ostream&
operator<<(std::ostream& os, const Instruction& i);

std::vector<Instruction>
flip_instruction(const std::vector<Instruction>& code, size_t index);

class ISS
{
public:
  ISS(std::vector<Instruction>& code)
    : code(code){};
  void     run_step(bool print_debug_info = false);
  uint32_t get_pc(void) const;
  int      get_acc(void) const;
  bool     sim_finish_unless_loop(void);

private:
  const std::vector<Instruction>& code;
  int                             acc = 0;
  uint32_t                        pc  = 0;
};

std::ostream&
operator<<(std::ostream& os, const ISS& iss);

int
main(void)
{
  std::vector<std::string> lines;
  std::string              line;
  while (std::getline(std::cin, line)) { lines.push_back(line); }

  std::vector<Instruction> code;
  std::transform(
    lines.begin(),
    lines.end(),
    std::back_inserter(code),
    [](const std::string& line) {
      return Instruction{ line.substr(0, 3), std::stoi(line.substr(4)) };
    });

  std::set<size_t> indices;
  for (size_t i = 0; i < code.size(); i++)
    if (code[i].opcode == "nop" || code[i].opcode == "jmp") indices.insert(i);

  for (size_t index : indices)
  {
    std::cout << "simulating with flip at " << index << std::endl;
    std::vector<Instruction> altered_code = flip_instruction(code, index);
    ISS                      iss(altered_code);

    if (iss.sim_finish_unless_loop())
    {
      std::cout << "final acc value: " << iss.get_acc() << std::endl;
      return EXIT_SUCCESS;
    }
  }

  return EXIT_FAILURE;
}

std::vector<Instruction>
flip_instruction(const std::vector<Instruction>& code, size_t index)
{
  std::vector<Instruction> altered_code(code);
  altered_code[index] = { (code[index].opcode == "jmp") ? "nop" : "jmp",
                          code[index].operand };
  return altered_code;
}

void
ISS::run_step(bool print_debug_info)
{
  if (print_debug_info) std::cout << (*this);

  const Instruction& instr = code[this->pc];

  if (print_debug_info) std::cout << " -> " << instr;

  if (instr.opcode == "nop") { this->pc++; }
  else if (instr.opcode == "acc")
  {
    this->acc += instr.operand;
    this->pc++;
  }
  else if (instr.opcode == "jmp")
  {
    if (instr.operand < 0 && std::abs(instr.operand) > (int)this->pc)
      std::cerr << "warning: PC underflow by adding " << instr.operand << " to "
                << this->pc << std::endl;
    this->pc += instr.operand;
  }
  else
  {
    if (print_debug_info) std::cout << std::endl;
    std::cerr << "error: unknown instruction: " << instr.opcode << std::endl;
    exit(EXIT_FAILURE);
  }

  if (print_debug_info) std::cout << " -> " << *this << std::endl;
}

uint32_t
ISS::get_pc(void) const
{
  return this->pc;
};

int
ISS::get_acc(void) const
{
  return this->acc;
};

bool
ISS::sim_finish_unless_loop(void)
{
  std::set<uint32_t> pc_values;
  while ((!pc_values.contains(this->pc)) && this->pc < this->code.size())
  {
    pc_values.insert(this->pc);
    this->run_step();
  }
  return this->pc >= this->code.size();
}

std::ostream&
operator<<(std::ostream& os, const ISS& iss)
{
  return os << '[' << std::setw(5) << iss.get_pc() << std::setw(5)
            << iss.get_acc() << ']';
}

std::ostream&
operator<<(std::ostream& os, const Instruction& i)
{
  return os << '(' << i.opcode << std::setw(5) << i.operand << ')';
}
