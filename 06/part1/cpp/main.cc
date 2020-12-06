#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

int
main(void)
{
  std::vector<std::set<char>> groups;

  std::set<char> group;
  std::string    line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      groups.push_back(group);
      group = {};
    }
    for (char c : line) group.insert(c);
  }
  if (!group.empty()) groups.push_back(group);

  std::cout << std::accumulate(
                 groups.begin(),
                 groups.end(),
                 0,
                 [](auto sum, const auto& g) { return sum + g.size(); })
            << std::endl;

  return EXIT_SUCCESS;
}
