#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

int
main(void)
{
  size_t across = 0;
  size_t tree_count = 0;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line[across] == '#') tree_count++;
    across = (across + 3) % 31;
  }

  std::cout << tree_count << std::endl;

  return EXIT_SUCCESS;
}
