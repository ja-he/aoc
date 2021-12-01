#include <algorithm>
#include <bitset>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#define ALL_YES 0b00000011111111111111111111111111
#define ALL_NO 0b00000000000000000000000000000000

/**
 * @brief counts the number of set bits in a given non-negative 32 bit integer
 *        by Brian Kernighan's algorithm.
 *
 * @param num a 32 bit non-negative integer
 * @return the number of set bits in the given number
 */
size_t
n_set_bits(uint32_t num);

int
main(void)
{
  // the groups on the flight, initialized with the first group's starting
  // value of assuming everybody says yes to each question, which will be
  // whittled down using the logical AND as we parse the input
  std::vector<uint32_t> groups{ ALL_YES };

  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty()) // if we found the break between groups...
    {
      groups.push_back(ALL_YES); // ... set up the next group and ...
      continue;                  // ... skip to the next group's first answer.
    }

    // a person who we presume says no to everything, until we parse their
    // answers and update the values using the logical OR
    uint32_t person = ALL_NO;

    // for each answer, set the corresponding bit to true
    for (const char& c : line) { person = person | (1 << (c - 'a')); }

    // update the group by filtering only for this last person's affirmative
    // answers
    groups.back() &= person;
  }

  // sum up the numbers of true answers (set bits) of each group and output it
  std::cout << std::accumulate(
                 groups.begin(),
                 groups.end(),
                 0,
                 [](auto sum, const auto& g) { return sum + n_set_bits(g); })
            << std::endl;

  return EXIT_SUCCESS;
}

size_t
n_set_bits(uint32_t num)
{
  size_t count = 0;
  while (num)
  {
    num &= (num - 1);
    count++;
  }
  return count;
}
