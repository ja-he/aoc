#include <iomanip>
#include <iostream>

#include <algorithm>
#include <numeric>

#include <bitset>
#include <map>
#include <string>
#include <vector>

#define WANTED_INDEX 2020

using Number = unsigned;

int
main(void)
{
  std::vector<Number> numbers;
  {
    std::string starting_number;
    while (std::getline(std::cin, starting_number, ','))
      numbers.push_back(std::stoul(starting_number));
  }

  while (numbers.size() < WANTED_INDEX)
  {
    Number current = numbers.back();
    auto   first_occurrence =
      std::find(numbers.rbegin() + 1, numbers.rend(), current);
    numbers.push_back((first_occurrence == numbers.rend())
                        ? 0
                        : first_occurrence - numbers.rbegin());
  }

  std::cout << "number at " << WANTED_INDEX << ": " << numbers.back()
            << std::endl;

  return EXIT_SUCCESS;
}
