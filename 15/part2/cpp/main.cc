#include <iomanip>
#include <iostream>

#include <algorithm>
#include <numeric>

#include <bitset>
#include <map>
#include <string>
#include <vector>

#define WANTED_INDEX 30000000

using Number = unsigned;
using Step   = uint32_t;

int
main(void)
{
  Step              index = 1;
  Number            current;
  std::vector<Step> numbers(WANTED_INDEX, 0);

  {
    std::vector<Number> starting_nums;
    std::string         starting_str;
    while (std::getline(std::cin, starting_str, ','))
    {
      starting_nums.push_back(std::stoul(starting_str));
    }
    std::for_each(starting_nums.begin(),
                  starting_nums.end() - 1,
                  [&index, &numbers](Number n) {
                    numbers[n] = index;
                    index++;
                  });
    current = starting_nums.back();
  }

  while (index < WANTED_INDEX)
  {
    Step last_occurrence = numbers[current];
    numbers[current]     = index;
    current              = (last_occurrence) ? index - last_occurrence : 0;
    index++;
  }

  std::cout << "number at " << WANTED_INDEX << ": " << current << std::endl;
  return EXIT_SUCCESS;
}
