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

#define NUMRANGE 25

using Value = long long;

int
main(void)
{

  std::vector<unsigned> xmas;
  Value n;
  while (std::cin >> n)
  {
    xmas.push_back(n);
  }

  for (auto it = xmas.begin()+NUMRANGE; it < xmas.end(); it++)
  {
    Value n = *it;
    std::set<Value> sums;
    for (auto it_i = it-NUMRANGE; it_i < it; it_i++)
      for (auto it_j = it-NUMRANGE; it_j < it; it_j++)
        if (*it_i != *it_j)
          sums.insert(*it_i+*it_j);
    if (!sums.contains(n))
    {
      std::cout << "answer: " << n << std::endl;
      return EXIT_SUCCESS;
    }
  }

  return EXIT_FAILURE;
}
