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
using Step   = unsigned;

int
main(void)
{
  int index = 1;
  Number current;
  std::unordered_map<Number, Step> numbers;

  {
    std::vector<Number> starting_nums;
    std::string starting_str;
    while (std::getline(std::cin, starting_str, ','))
    {
      starting_nums.push_back(std::stoul(starting_str));
    }
    std::for_each(starting_nums.begin(), starting_nums.end()-1,
      [&index,&numbers](Number n){ numbers[n] = index; index++; });
    current = starting_nums.back();
  }

  while (index < WANTED_INDEX)
  {
    if (numbers.contains(current)) {
      Step step = numbers.at(current);
      numbers[current] = index;
      current = index - step;
    } else {
      numbers[current] = index;
      current = 0;
    }

    index++;
  }

  std::cout << "number at " << WANTED_INDEX << ": " << current
            << std::endl;

  return EXIT_SUCCESS;
}
