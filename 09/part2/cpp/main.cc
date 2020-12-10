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
  Value                 n;
  while (std::cin >> n) { xmas.push_back(n); }

  Value answer;
  auto  answer_pos = xmas.begin();
  for (auto it = xmas.begin() + NUMRANGE; it < xmas.end(); it++)
  {
    Value           n = *it;
    std::set<Value> sums;
    for (auto it_i = it - NUMRANGE; it_i < it; it_i++)
      for (auto it_j = it - NUMRANGE; it_j < it; it_j++)
        if (*it_i != *it_j) sums.insert(*it_i + *it_j);
    if (!sums.contains(n))
    {
      std::cerr << "answer found: " << n << std::endl;
      answer     = n;
      answer_pos = it;
      break;
    }
  }

  if (answer_pos == xmas.begin())
  {
    std::cerr << "answer unexpectedly not found" << std::endl;
    return EXIT_FAILURE;
  }

  auto lb = xmas.begin();
  auto ub = xmas.begin() + 2;
  while (ub != xmas.end())
  {
    if (ub == answer_pos)
    {
      lb = answer_pos + 1;
      ub = answer_pos + 1 + 2;
      continue;
    }

    auto sum = std::accumulate(lb, ub, 0);

    if (sum < answer) { ub++; }
    else if (sum > answer)
    {
      lb++;
    }
    else
    {
      // contiguous sequence found
      break;
    }
  }

  auto weakness = *std::min_element(lb, ub) + *std::max_element(lb, ub);

  std::cout << "xmas weakness: " << weakness << std::endl;

  return EXIT_SUCCESS;
}
