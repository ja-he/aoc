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

class ISS
{
public:
  ISS(std::vector<Instruction>& code)
    : code(code){};
  void     run_step(void);
  uint32_t get_pc(void) const;
  int      get_acc(void) const;

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

  ISS                iss(code);
  std::set<uint32_t> executed_instructions;
  while (!executed_instructions.contains(iss.get_pc()))
  {
    executed_instructions.insert(iss.get_pc());
    iss.run_step();
  }
  std::cout << "acc = " << iss.get_acc() << std::endl;

  return EXIT_SUCCESS;
}

void
ISS::run_step(void)
{
  std::cout << (*this);

  const Instruction& instr = code[this->pc];
  std::cout << " -> " << instr;

  if (instr.opcode == "nop") { this->pc++; }
  else if (instr.opcode == "acc")
  {
    this->acc += instr.operand;
    this->pc++;
  }
  else if (instr.opcode == "jmp")
  {
    if (instr.operand < 0 && std::abs(instr.operand) > (int)this->pc)
      std::cerr << "warning: PC underflow by adding " << instr.operand << " to " << this->pc << std::endl;
    this->pc += instr.operand;
  }
  else
  {
    std::cout << std::endl;
    std::cerr << "error: unknown instruction: " << instr.opcode << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << " -> " << *this << std::endl;
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
