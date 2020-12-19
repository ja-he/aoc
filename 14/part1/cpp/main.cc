#include <iomanip>
#include <iostream>

#include <algorithm>
#include <numeric>

#include <bitset>
#include <map>
#include <string>
#include <vector>

#define MASK_SIZE 36

using MemoryValue = uint64_t;
using MemoryIndex = size_t;
using ValueIndex  = uint8_t;

class Memory
{
public:
  void        set_value(MemoryIndex i, MemoryValue v);
  MemoryValue sum_up_values(void) const;

private:
  std::map<MemoryIndex, MemoryValue> memory;
};

struct Instruction
{
  MemoryIndex index;
  MemoryValue value;
};

class InstructionBatch
{
public:
  void set_mask(const std::string& rval);
  void add_instruction(const std::string& lval, const std::string& rval);
  void execute_instructions(Memory& m) const;

private:
  MemoryValue              overrides_mask   = -1;
  MemoryValue              overrides_values = 0;
  std::vector<Instruction> instructions;
};

int
main(void)
{
  std::vector<InstructionBatch> v;
  {
    std::string lval, assigns, rval;
    while (std::cin >> lval >> assigns >> rval)
    {
      if (lval == "mask")
      {
        v.emplace_back();
        v.back().set_mask(rval);
      }
      else
      {
        v.back().add_instruction(lval, rval);
      }
    }
  }

  Memory m;
  for (const InstructionBatch& batch : v) { batch.execute_instructions(m); }

  MemoryValue result = m.sum_up_values();
  std::cout << result << std::endl;
  if (result == 2) std::cout << "its 2" << std::endl;

  return EXIT_SUCCESS;
}

void
InstructionBatch::set_mask(const std::string& rval)
{
  for (ValueIndex i = 0; i < MASK_SIZE; i++)
  {
    char b = rval[MASK_SIZE - 1 - i];
    if (b != 'X')
    {
      this->overrides_mask ^= ((MemoryValue)1 << i);
      this->overrides_values |= (((MemoryValue)(b == '1')) << i);
    }
  }
}

void
InstructionBatch::add_instruction(const std::string& lval,
                                  const std::string& rval)
{
  const size_t start = lval.find('[') + 1;
  const size_t end   = lval.find(']');
  this->instructions.push_back(
    { std::stoul(lval.substr(start, end - start)), std::stoul(rval) });
}

void
InstructionBatch::execute_instructions(Memory& m) const
{
  for (auto [mi, mv] : this->instructions)
  {
    mv &= this->overrides_mask;
    mv |= this->overrides_values;
    m.set_value(mi, mv);
  }
}

void
Memory::set_value(MemoryIndex i, MemoryValue v)
{
  this->memory[i] = v;
}

MemoryValue
Memory::sum_up_values(void) const
{
  return std::accumulate(
    this->memory.begin(),
    this->memory.end(),
    (MemoryValue)0,
    [](MemoryValue sum, const std::pair<MemoryIndex, MemoryValue>& pair) {
      return sum + pair.second;
    });
}
