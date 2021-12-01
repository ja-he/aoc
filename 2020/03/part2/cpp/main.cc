#include <algorithm>
#include <bitset>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#define LINE_LENGTH 31

std::vector<std::bitset<LINE_LENGTH>>
parse_input_for_landscape(void);

int
main(void)
{
  std::vector<std::bitset<LINE_LENGTH>> landscape = parse_input_for_landscape();

  // clang-format off
  std::vector<std::pair<size_t, size_t>> slopes{
    { 1, 1 },
    { 1, 3 },
    { 1, 5 },
    { 1, 7 },
    { 2, 1 }
  };
  // clang-format on

  std::vector<unsigned long long> tree_counts;

  for (const auto& [ds, as] : slopes) {
    size_t across     = 0;
    size_t tree_count = 0;

    size_t down_index = 0;

    for (const auto& line : landscape) {
      // increment down index and skip if appropriate
      if (down_index++ % ds != 0) {
        continue;
      }

      if (line[LINE_LENGTH - 1 - across]) {
        tree_count++;
      }

      // increment across index
      across = (across + as) % LINE_LENGTH;
    }

    tree_counts.push_back(tree_count);
  }

  unsigned long long product =
    std::reduce(tree_counts.begin(),
                tree_counts.end(),
                1,
                std::multiplies<unsigned long long>());
  std::cout << product << std::endl;

  return EXIT_SUCCESS;
}

std::vector<std::bitset<LINE_LENGTH>>
parse_input_for_landscape(void)
{
  std::vector<std::bitset<LINE_LENGTH>> result;
  std::string                           line;

  while (std::getline(std::cin, line)) {
    result.emplace_back(line, 0, 31, '.', '#');
  }

  return result;
}
